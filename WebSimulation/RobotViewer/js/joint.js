var IK_MAX_ITERATIONS = 1;
var IK_DISABLE_BACKWARD = true;

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

function print(name, text = '') {
    print_text(name, text);
}

function print_scalar(name, s) {
    str = pad(name, TEXT_PADDING, ' ', STR_PAD_RIGHT);
    console.log(" " + str + " "+ "[" + s.toFixed(2)+ "]");
}

//####################################
//# ThreeJS & Maths Help
//####################################

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

// Creates a line circle orientated with the current orientation of the world and rotated around an axis
function create_circle(group, center, vect_orientation, orientation, world_quaternion, scale, color_hex) {
    var resolution = 20;
    var amplitude = 25 * SCALE * scale;
    var size = 360 / resolution;

    var geometry = new THREE.Geometry();
    var material = new THREE.LineBasicMaterial( { color: color_hex, opacity: 1.0} );
    for(var i = 0; i <= resolution; i++) {
        var segment = ( i * size ) * Math.PI / 180;
        var vect = new THREE.Vector3();
        var cx = Math.cos( segment ) * amplitude;
        var cy = Math.sin( segment ) * amplitude;

        if (orientation == 'Y') {
            vect.set(cx, 0, cy);
        } else
        if (orientation == 'Z') {
            vect.set(cx, cy, 0);
        } else {
            vect.set(0, cx, cy);
        }

        geometry.vertices.push( vect );
    }

    print_vector("Orientation", vect_orientation);
    print_text("Orientation_text", orientation);

    var line = new THREE.Line( geometry, material );
    line.position.set(center.x, center.y, center.z);

    //var angleOfRotation = THREE.Math.degToRad( 90 );
    //var quaternion = new THREE.Quaternion().setFromAxisAngle( vect_orientation, angleOfRotation );
    //quaternion.multiply(world_quaternion);
    line.rotation.setFromQuaternion( world_quaternion );

    group.add(line);
}

function create_help_arrow(group, dir, origin, length, color_hex) {
    var arrowHelper = new THREE.ArrowHelper( dir, origin.clone(), length, color_hex );
    group.add( arrowHelper );
}

// Middle point helper, just draw a marker in the middle of the line between two points.
function create_help_mid_line(group, text, p0, p1, color) {
    var mid_pos = p1.clone();
    mid_pos.add(p0);
    mid_pos.multiplyScalar(0.5);
    create_marker(group, text, mid_pos, {color: 0xaaaaaaa}, 0.3);
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

// V1*V2=x1*x2 + y1*y2 + z1*z2
function vect_dot(v1, v2) {
    dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z ;
    return (dot);
}

// V1xV2=(y1*z2-z1*y2)i+(z1*x2-x1*z2)j+(x1*y2-y1*x2)k
function vect_cross(v1, v2) {
    var dest = new THREE.Vector3();
    dest.x = (v1.y*v2.z) - (v1.z*v2.y);
    dest.y = (v1.z*v2.x) - (v1.x*v2.z);
    dest.z = (v1.x*v2.y) - (v1.y*v2.x);
    return dest;
}

function vect_mod(v) {
    return (Math.sqrt(v.x*v.x + v.y*v.y + v.z*v.z));
}

function vect_get_angle(A, B) {
    angle = Math.acos( vect_dot(A,B) / (vect_mod(A) * vect_mod(B)));
    return angle;
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
        this.angle = 0;
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

        // Local joint rotation
        this.j_rotation = { x:0, y:0, z:0 };

        // World joint rotation
        this.w_rotation = { x:0, y:0, z:0 };
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
            this.obj_rotation = new THREE.Vector3(0,1,0);
            this.obj_up = new THREE.Vector3(0,0,1);
        } else
        if (this.orientation == 'Z') {
            this.obj_rotation = new THREE.Vector3(0,0,1);
            this.obj_up = new THREE.Vector3(1,0,0);
        } else {
            this.obj_rotation = new THREE.Vector3(1,0,0);
            this.obj_up = new THREE.Vector3(0,1,0);
        }
    }

    setAngle(rad) {
        this.angle += rad;
        if (!this.vect_rotation) {
            var w_quaternion = new THREE.Quaternion();
            this.getWorldQuaternion(w_quaternion);
            this.vect_rotation = this.obj_rotation.clone();
            this.vect_rotation.applyQuaternion(w_quaternion);
        }

        this.rotateOnAxis(this.vect_rotation, rad);
        this.scene.updateMatrixWorld();

        // TODO propagate rotation to the rest of the chain
        // We need to keep the rotations up to date
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
            //print_text("ROOT");
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

        print_text("Orientation", this.orientation);

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
        if (n < 1 || n > P.length - 1) {
            this.print();
            return;
        }

        this.vect_joint = get_vector(P[n-1], P[n]);

        var w_quaternion = new THREE.Quaternion();
        this.getWorldQuaternion(w_quaternion);

        // Original object transformation before rotation and world orientation
        // create_help_arrow(ik_group, this.obj_up, P[n-1], 5 * SCALE, 0x0000ff);
        // create_help_arrow(ik_group, this.obj_rotation, P[n-1], 5 * SCALE, 0xffffff);

        // Clone
        this.vect_rotation = this.obj_rotation.clone();
        this.vect_rotation.applyQuaternion(w_quaternion);

        this.vect_up = this.obj_up.clone();
        this.vect_up.applyQuaternion(w_quaternion);

        create_circle(ik_group, P[n - 1], this.vect_up, this.orientation, w_quaternion, 15 * SCALE, 0xffffff);

        /*
        var text = "Q[ " + w_quaternion.x.toFixed(1) + ", " + w_quaternion.y.toFixed(1) + ", " + w_quaternion.z.toFixed(1) + ", " + w_quaternion.w.toFixed(1) + " ]";
        var mesh  = robot.create_text(text, 0x00ff00, 1.5 , 1);
        mesh.position.set(P[n].x - 0.5 * SCALE, P[n].y, P[n].z + 2.5 * SCALE);
        mesh.visible = true;
        ik_group.add(mesh);
        */

        //create_help_arrow(ik_group, this.vect_up, P[n-1], 5 * SCALE, 0x0000ff);
        create_help_arrow(ik_group, this.vect_rotation, P[n-1], 7 * SCALE, 0x00ff00);
        create_help_arrow(ik_group, this.vect_joint, P[n-1], 3 * SCALE, 0xffffff);
    }

    ik_backward_point(ik_group, P, P_, n, limit) {
        var color = { color: 0xffffff };

        var point = P_[n];
        var pAttachment = P_[n - 1].clone();
        var vect = new THREE.Vector3();

        create_help_line(ik_group, point, pAttachment, color );

        //------------------------------------------------------------
        // Apply rotation constraints

        // We project our new point into our possible rotational plane

        if (n > 0) {
            var plane = new Plane();
            plane.create_from_normal(this.vect_rotation,  pAttachment);

            var projected_p = plane.project(point);
            create_marker(ik_group, "Pj " + n, projected_p, {color: 0xffffff}, 0.3);

            // Replace the point with the constrained point.
            point = projected_p;
        }

        //------------------------------------------------------------

        // Find next P(n) ''
        vect.set(point.x, point.y, point.z);

        vect.sub(pAttachment);
        vect.normalize();

        // Current vector of the joint
        //create_help_arrow(ik_group, vect, pAttachment, 25 * SCALE, 0x0000ff);

        vect.multiplyScalar(this.joint_length);

        vect.add(pAttachment);

        return vect;
    }

    ik_backward_step(ik_group, P, P_, n, iterations) {
        var joint = this;
        var point = P_[n]; //.clone();

        this.print();
        print_text("Ik_forward " + iterations ,  "["+n+"]");

        //var color = { color: 0xff + (n * 63 << 8) }
        var color = { color: 0xff0000 };

        P_[n] = this.ik_backward_point(ik_group, P, P_, n);
        create_marker(ik_group, "P" + (n) + s_prime(iterations), P_[n], color, 0.25);

        if (this.next_joint) {
            print_vector("Line P"+ s_prime(iterations) + (n - 1) ,  P_[n - 1]);
            print_vector("Line P"+ s_prime(iterations) + (n) ,  P_[n]);

            create_help_line(ik_group, P_[n], P_[n - 1], { color: 0xffff00 } );

            // Propagate into the next joints, our joints will be a tree structure.
            this.next_joint.ik_backward_step(ik_group, P, P_, n + 1, iterations);
        } else {
            print_h2("End effector " + iterations);

            // We don't have a next joint, so we calculate here since there is not going to be propagation
            P_[n] = this.ik_backward_point(ik_group, P, P_, n);

            print_h2("Finished " + iterations);

            create_help_line(ik_group, P_[n], P_[n - 1], { color: 0xffffff } );
            create_marker(ik_group, "P" + (n) +  s_prime(iterations), P_[n], {color: 0xffffff}, 0.3);
        }
    }

    ik_orientation_step(ik_group, target, P, n, iterations = 1) {
        if (n > 0)
            this.ik_create_helpers(ik_group, P, n);

        if (this.is_end_effector) {
            print_h2("Found end_effector");
            return;
        }

        // TODO Check if the index is OK, we should use the data from the joint and not the index.

        // Calculate rotation
        if (this.orientation == 'Z') {

            // 1. Find plane defined by the normal vector to the joint
            // 2. Project the target point into this plane
            // 3. Find the angle between the target and the joint vector on this plane

            var plane = new Plane();
            plane.create_from_normal(this.vect_rotation,  P[n - 1]);

            var projected_p = plane.project(target);
            create_marker(ik_group, "Pj " + n, projected_p, {color: 0xffffff}, 0.3);
            print_vector("Projected ", projected_p);

            var vect_proj_target = get_vector(P[n - 1], projected_p);

            create_help_line(ik_group, P[n - 1], projected_p, { color: 0xffffff } );
            create_help_arrow(ik_group, vect_proj_target, P[n - 1], 25 * SCALE, 0x00ffff);

            var angle = vect_get_angle(this.vect_joint, vect_proj_target);

            // Check sign of the rotation, use cross product of the two vectors to get the normal of the plane formed by the two vectors.
            // Then check the dotproduct between that and the original plane normal to see if they are facing the same direction.

            var cross = vect_cross(this.vect_joint, vect_proj_target);
            var dot = vect_dot(this.vect_rotation, cross);
            if (dot < 0) {
                angle = -angle;
            }

            print_scalar("Angle", THREE.Math.radToDeg(angle));
            this.setAngle(angle);
        }

        var next = this.next_joint;
        next.ik_orientation_step(ik_group, target, P, n + 1, iterations);
        P[n] = get_world_position(this);
    }

    ik_forward_step(ik_group, P, P_, n, iterations = 1) {

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
        // This line will define our new vector in which the new position forward will lie
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

        //TODO: Apply new angle for this joint and check limits

        //------------------------------------------------------------

        // Apply rotation constraints
        // We project our new point into our possible rotational plane
/*
        var plane = new Plane();
        plane.create_from_normal(this.vect_rotation,  pAttachment);

        var projected_p = plane.project(point);
        create_marker(ik_group, "Pj " + n, projected_p, {color: 0xffffff}, 0.3);

        // Replace the point with the constrained point.
        p = projected_p;
*/
        //------------------------------------------------------------
        P_[n - 1] = p.clone();

        create_help_line(ik_group, p, pAttachment, color );

        if (joint.parent_joint) {
            joint.parent_joint.ik_forward_step(ik_group, P, P_, n - 1, iterations);
        } else {
            console.log("End forward");

            //--------------------------------------
            // Backward IK
            print_h1("BACKWARD");
            iterations++;

            P_[0] = P[0];
            if (! IK_DISABLE_BACKWARD )
                this.ik_backward_step(ik_group, P, P_, n, iterations);
        }

        return ik_group;
    }

    //-----------------------------------------------------------
    // FABRIK forward propagation implementation
    //-----------------------------------------------------------

    generate_world_positions(ik_group, P, P_, n) {
        // First pass to populate our array of points
        var next = this;

        P[n] = get_world_position(next.link_prev);
        create_marker(ik_group, "P" + n, P[n], {color: 0x00ff00}, 0.2);
        n++;

        while (!next.is_end_effector) {
            next = next.next_joint;
            P[n] = get_world_position(next.link_prev);
            create_marker(ik_group, "P" + n, P[n], {color: 0x00ff00}, 0.2);
            n++;
        };

        // The last joint position is the next link
        P[n] = get_world_position(next.link_next);
        create_marker(this.root_ik, "P" + n, P[n], {color: 0x00ff00}, 0.2);

        return next;
    }

    fabrik_forward(target) {
        if (this.root_ik)
            scene.remove(this.root_ik);

        this.root_ik = new THREE.Group();

        // P array with real joint precalculated values of world coordinates per joint attachment
        var P = new Array();

        // P' array for forward pass
        var P_ = new Array();

        // The last link is our end effector from which we will back propagate
        var end_effector = this.generate_world_positions(this.root_ik, P, P_, 0);

        // Our P' will be forward propagate from here

        var n = P.length - 1;
        P_[n] = target.clone();
        create_marker(this.root_ik, "P" + (n) + "'", P_[n], {color: 0x00ff00}, 0.2);

        //--------------------------------------
        // Forward IK

        // TODO: Loop until happy with IK

        return;

        this.ik_orientation_step(this.root_ik, target, P, 1, 1);

        // Recursive step forward from the root
        for (var i = 0; i < IK_MAX_ITERATIONS; i++) {
            this.generate_world_positions(this.root_ik, P, P_, 0);

            // From the end effector recursively calculate to the root and back
            end_effector.ik_forward_step(this.root_ik, P, P_, n, (i * 2) + 1);
            P = P_;
            P_ = new Array();
            P_[n] = target.clone();

            // Check angles and adapt angles to positions
        }

        scene.add(this.root_ik);
        render();
    }

    //-----------------------------------------------------------
    // Rotate joins Euler, propagate transformation
    //-----------------------------------------------------------

    rotate_joint_euler(x, y, z) {
        this.rotation.set( x, y, z );

        // Local joint rotation
        this.j_rotation.x = x;
        this.j_rotation.y = y;
        this.j_rotation.z = z;
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

        this.fabrik_forward(new_point);
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
