//####################################
//# Utils
//####################################

var STR_PAD_LEFT = 1;
var STR_PAD_RIGHT = 2;
var STR_PAD_BOTH = 3;

function pad(str, len, pad, dir) {

    if (typeof(len) == "undefined") { var len = 0; }
    if (typeof(pad) == "undefined") { var pad = ' '; }
    if (typeof(dir) == "undefined") { var dir = STR_PAD_RIGHT; }

    if (len + 1 >= str.length) {

        switch (dir){

            case STR_PAD_LEFT:
                str = Array(len + 1 - str.length).join(pad) + str;
            break;

            case STR_PAD_BOTH:
                var right = Math.ceil((padlen = len - str.length) / 2);
                var left = padlen - right;
                str = Array(left+1).join(pad) + str + Array(right+1).join(pad);
            break;

            default:
                str = str + Array(len + 1 - str.length).join(pad);
            break;

        } // switch

    }

    return str;
}

function print_vector(name, v) {
    str = pad(name, 16, ' ', STR_PAD_RIGHT);
    console.log(" " + str + " "+
    "(" + v.x.toFixed(2)+ ","
        + v.y.toFixed(2) + ","
        + v.z.toFixed(2) + ")");
}

function print_scalar(name, s) {
    str = pad(name, 16, ' ', STR_PAD_RIGHT);
    console.log(" " + str + " "+ "[" + s.toFixed(2)+ "]");
}

//####################################
//# ThreeJS & Maths Help
//####################################

function get_vector(p0, p1) {
    var pout = p1.clone();
    pout.sub(p0);
    pout.normalize();
    return pout;
}

function create_marker(group, text, position, color) {
    var mesh  = robot.create_text(text, 0xeeeeee, 5, 1);
    mesh.position.set(0, 5 * SCALE, 0);
    mesh.visible = true;

    var marker = create_pivot_geometry(color);
    marker.add(mesh);
    marker.position.set(position.x, position.y, position.z);
    group.add(marker);
    return marker;
}

function create_help_arrow(group, dir, origin, length, color_hex) {
    var arrowHelper = new THREE.ArrowHelper( dir, origin, length, color_hex );
    group.add( arrowHelper );
}

function create_help_line(group, p0, p1, color) {
    var material = new THREE.LineBasicMaterial( color );
    var geometry = new THREE.Geometry();
    geometry.vertices.push(p0);
    geometry.vertices.push(p1);
    var line = new THREE.Line( geometry, material );
    line.ignore_collision = true;
    group.add(line);
    return line;
}

function removeEntity(object) {
    var selectedObject = scene.getObjectByName(object.name);
    scene.remove( selectedObject );
    animate();
}

function get_world_position(obj) {
    var vector = new THREE.Vector3();
    vector.setFromMatrixPosition( obj.matrixWorld );
    return vector;
}

//####################################
//# Plane
//####################################

// Plane = Ax + By + Cz + D;
class Plane extends THREE.Plane {
    // Creates a plane from a normal and a point in 3D space.
    create_from_normal(normal, point) {
        this.normal = normal.clone();
        this.constant = - normal.dot(point);
    }

    // Calculates the distance from a plane to a point in 3d space
    // Given a plane (a, b, c, d) and a 3-D vector (x, y, z) the return value of this function is a*x + b*y + c*z + d*1.
    distance_to(point) {
        return (this.normal.x*point.x+this.normal.y*point.y+this.normal.z*point.z+this.constant);
    }

    project(point) {
        var distance = this.distance_to(point);
        var new_point = point.clone();
        var vect_plane = this.normal.clone();
        vect_plane.multiplyScalar(distance);
        new_point.sub(vect_plane);
        return new_point;
    }
}

//####################################
//# Joint
//####################################

class Joint extends THREE.Object3D {
    constructor(scene, name, geometry) {
        super();
        this.scene = scene;
        this.name = name;
        this.next = new Array();
        this.init();

        var geo = geometry.clone();

        this.add(geo);
        this.obj = geo;
        this.createOutline();
        this.next_joint = null;

        this.setOrientation('Y');
        this.counter = 0;
        this.is_end_effector = true;
    }

    create_text() {
        var vector = new THREE.Vector3();
        vector.setFromMatrixPosition( this.matrixWorld );

        var mesh  = robot.create_text("("+vector.x.toFixed(2)
                + "," + vector.y.toFixed(2)
                + "," + vector.z.toFixed(2)+")", 0xeeeeee, 5, 1);

        mesh.position.set(vector.x, vector.y + 1 * SCALE, vector.z);
        this.text_vector = vector;
        this.text_mesh = mesh;

        mesh.follow = this;
        this.scene.add(mesh);
    }

    update_text() {
        var vector = get_world_position(this);
        var old_vector = this.text_vector;

        if (vector.x == old_vector.x && vector.y == old_vector.y && vector.y == old_vector.y) {
            this.text_mesh.position.set(vector.x, vector.y + 1 * SCALE, vector.z);
        } else {
            this.text_mesh.geometry.dispose();
            this.text_mesh.material.dispose();

            this.scene.remove(this.text_mesh);
            this.text_mesh = undefined;

            this.create_text();
        }
    }

    setOrientation(value) {
        this.orientation = value;

        if (this.orientation == 'Y') {
            this.vect_rotation = new THREE.Vector3(0,1,0);
            this.vect_up = new THREE.Vector3(0,0,1);
        } else
        if (this.orientation == 'Z') {
            this.vect_rotation = new THREE.Vector3(0,0,1);
            this.vect_up = new THREE.Vector3(1,0,0);
        } else {
            this.vect_rotation = new THREE.Vector3(1,0,0);
            this.vect_up = new THREE.Vector3(0,1,0);
        }
    }

    setAngle(value) {
        if (this.orientation == 'Y') {
            this.rotation.set( 0, value * ( Math.PI/180 ),  0);
        } else {
            this.rotation.set( 0,0, value * ( Math.PI/180 ));
        }
    }

    createOutline() {
        var helper = new THREE.BoxHelper(this, 0x0000ff);
        helper.ignore_collision = true;

        this.add(helper);
        this.outline = helper;
        this.outline.visible = false;
    }

    init() {
        this.link_prev = create_pivot_geometry({color: 0xffff00});
        this.link_next = create_pivot_geometry({color: 0xff0000});

        this.add(this.link_prev);
        this.add(this.link_next);

        this.link_next.visible = false;
        this.link_prev.visible = false;
        this.create_text();
    }

    post_init() {
        create_help_line(this, this.link_prev.position, this.link_next.position, { color: 0x0000ff } );
    }

    setNextPivotPosition(x, y, z) {
        this.link_next.position.x = x;
        this.link_next.position.y = y;
        this.link_next.position.z = z;
    }

    setPivotVisibility(value) {
        this.link_next.visible = value;
        this.link_prev.visible = value;
    }

    attach(next_joint) {
        var pivot = this.link_next;
        next_joint.position.set(pivot.position.x, pivot.position.y, pivot.position.z);
        this.add(next_joint);
        this.next_joint = next_joint;
        this.next_joint.parent_joint = this;

        this.is_end_effector = false;
    }

    setTransparent(value) {
        this.transparent = value;
        for (var i in this.obj.children ) {
            var mesh = this.obj.children[i];
            if (mesh.userData.transparent) {
                mesh.visible = value;
            } else {
                mesh.visible = !value;
            }
        }
    }

    calculateLengthJoint() {
        var v1 = this.link_prev.position;
        var v2 = this.link_next.position;

        this.joint_length = v1.distanceTo( v2 );
        return this.joint_length;
    }

    calculateTotalDistance() {
        var length = this.calculateLengthJoint();

        if (this.next_joint)
            length += this.next_joint.calculateTotalDistance()

        console.log("Distance " + this.name + " = " + this.joint_length + " Total " + length );
        return length;
    }

    print() {
        console.log("---------- " + this.name + " ---------------");
        if (this.is_end_effector) {
            console.log("- End effector -");
        } else {
            console.log("- Joint -");
        }

        print_scalar("Length", this.joint_length)
        print_vector("Attachment", get_world_position(this.link_prev))
        print_vector("Next", get_world_position(this.link_next))
    }

}

//####################################
//# Leg
//####################################

class Leg extends Joint {
    constructor(scene, name, geometry) {
        console.log("---- Leg load ----")
        super(scene, name, geometry);
    }

    calculateTotalLegLength(self) {
        // Precalculate the local length for this joint
        this.calculateLengthJoint();

        // Propagate the calculation to the next joints
        this.total_length = this.next_joint.calculateTotalDistance();
        return this.total_length;
    }

    calculateDistanceToPoint(v2) {
        var v1 = this.link_next.position;
        var vector = new THREE.Vector3();
        vector.setFromMatrixPosition( this.link_next.matrixWorld );

        var distance = vector.distanceTo( v2 );
        return distance;
    }

    fabrik_backward_step(joint, P, P_, n) {

    }

    //-----------------------------------------------------------
    // FABRIK Backward propagation implementation
    //-----------------------------------------------------------

    fabrik_backward(point) {
        var fabrik_group = new THREE.Group();

        if (this.fabrik_group) {
            scene.remove(this.fabrik_group);
        }

        this.fabrik_group = fabrik_group;

        // P' array for backward pass

        var P = new Array();
        var P_ = new Array();
        var n = 0;
        var next = this.next_joint;
        P[n] = get_world_position(next.link_prev);
        create_marker(fabrik_group, "P" + n, P[n], {color: 0xffaaff});
        n++;

        while (!next.is_end_effector) {
            next = next.next_joint;
            P[n] = get_world_position(next.link_prev);
            create_marker(fabrik_group, "P" + n, P[n], {color: 0xaaaaff});
            n++;
        };

        P[n] = get_world_position(next.link_next);
        create_marker(fabrik_group, "P" + n, P[n], {color: 0xffaaff});

        if (this.orientation == 'Y') {

        }

        //fabrik_backward_step(next, P, P_, n)

        next.vect_joint = get_vector(P[n-1], P[n]);

        create_help_arrow(fabrik_group, next.vect_up, P[n-1], 25 * SCALE, 0xff0000);
        create_help_arrow(fabrik_group, next.vect_rotation, P[n-1], 25 * SCALE, 0xffff00);
        create_help_arrow(fabrik_group, next.vect_joint, P[n-1], 25 * SCALE, 0xffffff);

        var end_effector = next;
        end_effector.print();

        create_marker(fabrik_group, "P" + (n) + "'", point, {color: 0xaaaaff});

        // We define the point P(n) as our end_effector position
        // target position, from now on: P(n)'
        //
        // We have to trace a line with P(n-1) to P(n)'

        // Position of the attachment of the previous joint
        var pAttachment = get_world_position(end_effector.link_prev);
        var p = pAttachment.clone();

        // End effector position
        //var p_EF = get_world_position(end_effector.link_next);
        //create_marker(fabrik_group, "EF", p_EF, {color: 0xaaaaff});

        print_vector("[EF attachment]", p);

        create_help_line(fabrik_group, point, pAttachment, { color: 0xffff00 } );

        //------------------------------------------------------------------
        // This line will define our new vector in which the new position backward will lie
        // Position without a constraint
        // *------- P(n-1) <------- P(n)'

        p.sub(point);
        print_vector("P(" + (n-1) + ") - P("+ n +")' =", p);
        p.normalize();
        p.multiplyScalar(end_effector.joint_length);

        // We position the new line in the vector
        // New position is |P(n) - P(n - 1)| * newline_vector + P(n)'

        // Add the position of the target end
        p.add(point);
        create_marker(fabrik_group, "* P" + (n - 1) + "'", p, {color: 0xaaaaff});

        //------------------------------------------------------------------
        // JOINTS RESTRICTED
        // Create the vector defined by the project point and the previous point

        // To create the vector of the plane of the joint we have to find the normal to the plane.
        // We have the joint vector and a vector "up" created when we defined the rotation
        // This vector is the cross vector of both.

        var vector_normal_plane = new THREE.Vector3();
        vector_normal_plane.crossVectors(next.vect_joint, next.vect_up);
        create_help_arrow(fabrik_group, vector_normal_plane, P[n-1], 25 * SCALE, 0x0000ff);

        var plane = new Plane();
        plane.create_from_normal(vector_normal_plane,  P[n-1]);

        var projected_p = plane.project(point);
        create_marker(fabrik_group, "Proj", projected_p, {color: 0xffffff});

        // Middle point of the joint to position the plane
        var mid_pos = P[n-1].clone();
        mid_pos.add(P[n]);
        mid_pos.multiplyScalar(0.5);
        create_marker(fabrik_group, "middle", mid_pos, {color: 0xaaaaaaa});

        //var helper = new THREE.PlaneHelper( plane, 2, 0xffff00 );
        //helper.translateOnAxis(mid_pos, 1);
        //fabrik_group.add(helper);

        create_help_line(fabrik_group, projected_p, pAttachment, { color: 0xffff00 } );

        p = pAttachment.clone();
        p.sub(projected_p);
        print_vector("P(" + (n-1) + ") - P("+ n +")' =", p);

        p.normalize();
        print_vector("Normalized", p);

        create_help_arrow(fabrik_group, p, projected_p, 25 * SCALE, 0xff00ff);

        // Scale the position by the scalar of the joint length
        p.multiplyScalar(end_effector.joint_length);
        p.add(projected_p);

        print_vector("New point P("+ n + "-1)'", p);

        create_marker(fabrik_group, "P" + (n - 1) + "'", p, {color: 0xaaaaff});
        create_help_line(fabrik_group, projected_p, p, { color: 0xffff00 } );

        scene.add(fabrik_group);

        render();
    }

    calculateNewPosition(point) {
        var new_point = point.clone();

        var distance = this.calculateDistanceToPoint(new_point);

        if (distance > this.total_length) {
            console.log(" Outside reach " + this.name + " = " + distance.toFixed(2));
        } else {
            console.log(" Distance " + this.name + " = " + distance.toFixed(2));
        }

        this.fabrik_backward(new_point);
    }
}
