//####################################
//# Utils
//####################################

var STR_PAD_LEFT = 1;
var STR_PAD_RIGHT = 2;
var STR_PAD_BOTH = 3;

var TEXT_PADDING = 30;

function s_prime(iterations, marker = "'") {
    var str = '';
    for (var c=0; c < iterations; c++)
        str = str + marker;

    return str;
}

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

function print_h1(text) {
    var high = pad("", TEXT_PADDING, '#', STR_PAD_BOTH);
    console.log( high );
    var str = pad(" " + text + " ", TEXT_PADDING, '#', STR_PAD_BOTH);
    console.log( str );
    console.log( high );
}

function print_h2(text) {
    var str = pad(" " + text + " ", TEXT_PADDING, '-', STR_PAD_BOTH);
    console.log( str );
}

function print_vector(name, v) {
    str = pad(name, TEXT_PADDING, ' ', STR_PAD_RIGHT);
    console.log(" " + str + " "+
    "(" + v.x.toFixed(2)+ ","
        + v.y.toFixed(2) + ","
        + v.z.toFixed(2) + ")");
}

function print_text(name, text = '') {
    str = pad(name, TEXT_PADDING, ' ', STR_PAD_RIGHT);
    console.log(" " + str + " "+ "'" + text + "'");
}

function print_scalar(name, s) {
    str = pad(name, TEXT_PADDING, ' ', STR_PAD_RIGHT);
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

function create_marker(group, text, position, color, maker_scale = 0.4) {
    var mesh  = robot.create_text(text, color.color, 2 , 0.5 * maker_scale * 2);
    mesh.position.set(- 0.5 * SCALE, 0, 2.5 * SCALE);
    mesh.visible = true;

    var marker = create_pivot_geometry(color, maker_scale);
    marker.add(mesh);
    marker.position.set(position.x, position.y, position.z);
    group.add(marker);
    return marker;
}

function create_help_arrow(group, dir, origin, length, color_hex) {
    var arrowHelper = new THREE.ArrowHelper( dir, origin.clone(), length, color_hex );
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

// |A·B| = |A| |B| COS(θ)
// |A×B| = |A| |B| SIN(θ)
// return Math.Atan2(Cross(A,B), Dot(A,B));

function get_angle(A, B) {
    var angle = A.angleTo(B);
    return angle;
}

function get_angleX(A, B) {
    var vector = new THREE.Vector3();
    vector.set(A.x, 0, 0);
    return vecto.angleTo(B);
}

function get_angleY(A, B) {
    var vector = new THREE.Vector3();
    vector.set(0, A.y, 0);
    return vecto.angleTo(B);
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

        var mesh  = robot.create_text(this.name, 0xeeeeee, 1.5, 1);
        mesh.position.set(vector.x, vector.y, vector.z + 5 * SCALE);

        this.scene.add(mesh);

        var mesh  = robot.create_text("("+vector.x.toFixed(1)
                + "," + vector.y.toFixed(1)
                + "," + vector.z.toFixed(1)+")", 0xeeeeee, 3, 1);

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
        if (this.isRoot) {
            print_text("ROOT");
        }

        var pivot = this.link_next;
        next_joint.position.set(pivot.position.x, pivot.position.y, pivot.position.z);
        this.add(next_joint);
        this.next_joint = next_joint;

        next_joint.parent_joint = this;
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

        //console.log("Distance " + this.name + " = " + this.joint_length + " Total " + length );
        return length;
    }

    print() {
        print_h2(this.name);

        if (this.is_end_effector) {
            console.log(" End effector");
        } else {
            console.log(" Joint");
        }

        print_scalar("Length", this.joint_length)

        var name;

        if (this.parent_joint)
            name = "[" + this.parent_joint.name + "]"; else name = "NULL";

        print_vector("Attachment " + name, get_world_position(this.link_prev))

        if (this.next_joint)
            name = "[" + this.next_joint.name + "]"; else name = "NULL";

        print_vector("Next " + name, get_world_position(this.link_next))
    }

    // We have to step until we find all the end_effectors on the other side.
    // We should have a tree for the joints instead of an array or just past the next value and previous value

    ik_create_helpers(ik_group, P, n) {
        this.print();
        this.vect_joint = get_vector(P[n-1], P[n]);

        //create_help_arrow(ik_group, this.vect_up, P[n-1], 25 * SCALE, 0xff0000);
        //create_help_arrow(ik_group, this.vect_rotation, P[n-1], 25 * SCALE, 0xffff00);
        //create_help_arrow(ik_group, this.vect_joint, P[n-1], 25 * SCALE, 0xffffff);
    }

    ik_forward_point(ik_group, P, P_, n) {
        var color = { color: 0xffffff };

        var point = P_[n]; //.clone();
        var pAttachment = P_[n - 1].clone();
        create_help_line(ik_group, point, pAttachment, color );

        // Find next P(n) ''
        var vect = new THREE.Vector3();
        vect.set(point.x, point.y, point.z);

        vect.sub(pAttachment);
        vect.normalize();
        create_help_arrow(ik_group, vect, pAttachment, 25 * SCALE, 0x0000ff);

        vect.multiplyScalar(this.joint_length);

        vect.add(pAttachment);

        return vect;
    }

    ik_forward_step(ik_group, P, P_, n, iterations) {
        var joint = this;
        var point = P_[n]; //.clone();

        print_text("Ik_forward " + iterations ,  "["+n+"]");

        //var color = { color: 0xff + (n * 63 << 8) }
        var color = { color: 0xff0000 };

        P_[n] = this.ik_forward_point(ik_group, P, P_, n);
        create_marker(ik_group, "P" + (n) + s_prime(iterations), P_[n], color, 0.25);

        if (this.next_joint) {
            print_vector("Line P"+ s_prime(iterations) + (n - 1) ,  P_[n - 1]);
            print_vector("Line P"+ s_prime(iterations) + (n) ,  P_[n]);

            create_help_line(ik_group, P_[n], P_[n - 1],  { color: 0xffff00 } );
            this.next_joint.ik_forward_step(ik_group, P, P_, n + 1, iterations);
        } else {
            print_h2("End effector " + iterations);
            P_[n] = this.ik_forward_point(ik_group, P, P_, n);

            print_h2("Finished " + iterations);
            create_help_arrow(ik_group, P_[n], P_[n-1], 25 * SCALE, 0xffffff);
            create_marker(ik_group, "P" + (n) +  s_prime(iterations), P_[n], {color: 0xffffff}, 0.3);
        }
    }

    ik_backward_step(ik_group, P, P_, n, iterations = 1) {
        //[TODO] Next step is to propagate to the array of joints attached to the body.
        if (n == 0) {
            console.log("Reached end of IK")
            return;
        }

        var joint = this;
        var point = P_[n];

        var color = { color: 0xffff + n * 63 }

        this.ik_create_helpers(ik_group, P, n);

        // We define the point P(n) as our end_effector position
        // target position, from now on: P(n)'
        //
        // We have to trace a line with P(n-1) to P(n)'

        // Position of the attachment of the previous joint
        var pAttachment = get_world_position(this.link_prev);
        var p = pAttachment.clone();

        create_help_line(ik_group, point, pAttachment, color );

        //------------------------------------------------------------------
        // This line will define our new vector in which the new position backward will lie
        // Position without a constraint
        // *------- P(n-1) <------- P(n)'

        p.sub(point);
        p.normalize();
        p.multiplyScalar(this.joint_length);

        // We position the new line in the vector
        // New position is |P(n) - P(n - 1)| * newline_vector + P(n)'

        // Add the position of the target end
        p.add(point);
        create_marker(ik_group, "P" + (n - 1) + s_prime(iterations), p, color, 0.2);
        P_[n - 1] = p.clone();

        //TODO: Apply new angle for this joint and check limits

        //TODO: Apply constraints

        create_help_line(ik_group, p, pAttachment, color );

        if (joint.parent_joint) {
            joint.parent_joint.ik_backward_step(ik_group, P, P_, n - 1, iterations);
        } else {
            console.log("End backward");

            //--------------------------------------
            // Forward IK
            print_h2("FORWARD");
            iterations++;

            P_[0] = P[0];
            this.ik_forward_step(ik_group, P, P_, n, iterations);
        }

        return ik_group;
    }

    //-----------------------------------------------------------
    // FABRIK Backward propagation implementation
    //-----------------------------------------------------------

    fabrik_backward(point) {
        if (this.root_ik)
            scene.remove(this.root_ik);

        this.root_ik = new THREE.Group();

        // P array with real joint precalculated values of world coordinates per joint attachment
        var P = new Array();

        // P' array for backward pass
        var P_ = new Array();
        var n = 0;

        // First pass to populate our array of points
        var next = this;
        P[n] = get_world_position(next.link_prev);
        create_marker(this.root_ik, "P" + n, P[n], {color: 0x00ff00}, 0.2);
        n++;

        while (!next.is_end_effector) {
            next = next.next_joint;
            P[n] = get_world_position(next.link_prev);
            create_marker(this.root_ik, "P" + n, P[n], {color: 0x00ff00}, 0.2);
            n++;
        };

        // The last link is our end effector from which we will back propagate
        var end_effector = next;

        // The last joint position is the next link
        P[n] = get_world_position(end_effector.link_next);
        create_marker(this.root_ik, "P" + n, P[n], {color: 0x00ff00}, 0.2);

        // Our P' will be backward propagate from here
        P_[n] = point.clone();
        create_marker(this.root_ik, "P" + (n) + "'", P_[n], {color: 0x00ff00}, 0.2);

        //--------------------------------------
        // Backward IK

        // TODO: Loop until happy with IK

        // Recursive step backward

        for (var i = 0; i < 3; i++) {
            end_effector.ik_backward_step(this.root_ik, P, P_, n, (i * 2) + 1);
            P = P_;
            P_ = new Array();
            P_[n] = point.clone();
        }

        scene.add(this.root_ik);
        render();
    }

}

//####################################
//# Leg
//####################################

class Leg extends Joint {
    constructor(scene, name, geometry) {
        console.log("---- Leg load ----")
        super(scene, name, geometry);
        this.isRoot = true;
        this.attachments = new Array();
    }

    solve_ik(point) {
        var new_point = point.clone();

        var distance = this.calculateDistanceToPoint(new_point);

        print_h1("IK Solver");

        if (distance > this.total_length) {
            console.log(" Outside reach " + this.name + " = " + distance.toFixed(2));
        } else {
            console.log(" Distance " + this.name + " = " + distance.toFixed(2));
        }

        this.fabrik_backward(new_point);
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

    // We have an array of legs in which we are attached so we can backpropagate the IK
    attach_root(leg) {
        this.attachments.push(leg);
        for (var i = 0; i < this.attachments.length; i++) {
            //console.log(this.attachments[i]);
        }
    }

    /*
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
    */
}
