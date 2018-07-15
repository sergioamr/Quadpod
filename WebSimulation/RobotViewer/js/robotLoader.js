if ( ! Detector.webgl ) Detector.addGetWebGLMessage();

var camera, scene, renderer;
var world = new THREE.Group();

var SCALE = 0.01;
var PIVOT_SIZE = 2.5 * SCALE;

var controls;

function render() {
    //manageRaycasterIntersections(scene, camera);
    renderer.render( scene, camera );
}

function animate() {
    for (var i in robot.font_meshes) {
        var font = robot.font_meshes[i];

        if (font.follow)
            font.follow.update_text();

        font.quaternion.copy( camera.quaternion );
    }

    render();
    requestAnimationFrame(animate);
}

// obj - your object (THREE.Object3D or derived)
// point - the point of rotation (THREE.Vector3)
// axis - the axis of rotation (normalized THREE.Vector3)
// theta - radian value of rotation
// pointIsWorld - boolean indicating the point is in world coordinates (default = false)
function rotateAboutPoint(obj, point, axis, theta, pointIsWorld) {
    pointIsWorld = (pointIsWorld === undefined)? false : pointIsWorld;

    if(pointIsWorld){
        obj.parent.localToWorld(obj.position); // compensate for world coordinate
    }

    obj.position.sub(point); // remove the offset
    obj.position.applyAxisAngle(axis, theta); // rotate the POSITION
    obj.position.add(point); // re-add the offset

    if(pointIsWorld){
        obj.parent.worldToLocal(obj.position); // undo world coordinates compensation
    }

    obj.rotateOnAxis(axis, theta); // rotate the OBJECT
}

var shell = new THREE.Object3D();
var pelvis = new THREE.Object3D();
var joint_mesh_1 = new THREE.Object3D();
var joint_mesh_2 = new THREE.Object3D();
var joint_mesh_3 = new THREE.Object3D();
var joint_mesh_4 = new THREE.Object3D();
var foot_mesh = new THREE.Object3D();

var loaded_meshes = 0;
var total_meshes = 2;

class Robot extends THREE.Group {
    constructor() {
        super();
        this.name = 'Spider';
        this.state = 'Loading';
        this.display_outline = false;
        this.show_joints = false;
        this.transparent = true;
        this.shell = null;
        this.legs = new Array();
        this.font_meshes = new Array();
        this.add(create_pivot_geometry({color: 0xffffff}, 0.1));
        this.font_loader();
    }

    font_loader() {
        var loader = new THREE.FontLoader();
        loader.load( 'fonts/droid_sans_regular.typeface.json',
            function ( font ) {
                robot.font = font;
                finished_loading();
            }
        );
    }

    create_static(text, color, size, height) {
        var textGeometry = new THREE.TextGeometry( text, {
            font: this.font,
            size: size * SCALE,
            height: height * SCALE,
            curveSegments: 12,
            bevelEnabled: false,
            bevelThickness: 10 * SCALE,
            bevelSize: 8 * SCALE,
            bevelSegments: 5
        } );

        var textMaterial = new THREE.MeshPhongMaterial( { color: color, specular: 0xffffff, emissive: 0xffffff, shininess: 200  } );
        var mesh = new THREE.Mesh( textGeometry, textMaterial );
        return mesh;
    }

    create_text(text, color, size, height) {
        var mesh = this.create_static(text, color, size, height);
        this.font_meshes.push(mesh);
        mesh.visible = false;
        return mesh;
    }

    setOutline(value) {
        for (var i in this.legs) {
            var leg = this.legs[i];
            for (var c in leg.outlines) {
                var outline = leg.outlines[c];
                outline.visible = value;
            }
        }
        render();
    }

    showJoints(value) {
        if (value && this.transparent == false)
            this.showTransparent(true);

        for (var i in this.legs) {
            var leg = this.legs[i];
            var next = leg.next_joint;

            while (next != null) {
                next.setPivotVisibility(value);
                next = next.next_joint;
            }
        }

        for (var i in this.font_meshes) {
            var text = this.font_meshes[i];
            text.visible = value;
        }

        render();
    }

    setTransparency(obj, value) {
        for (var i in obj.children ) {
            var mesh = obj.children[i];
            if (mesh.userData.transparent != undefined) {
                if (mesh.userData.transparent) {
                    mesh.visible = value;
                } else {
                    mesh.visible = !value;
                }

            } else {
                this.setTransparency(mesh, value);
            }
        }
    }

    showTransparent(value) {
        this.transparent = value;
        this.setTransparency(this, value);
        render();
    }

    setPose(value) {
        if (value == "calibration") {
            for (var i in this.legs) {
                var leg = this.legs[i];
                var next = leg.next_joint;
                while (next != null) {
                    next.setAngle(0);
                    next = next.next_joint;
                }
            }

        } else
        if (value == "relax") {
            for (var i in this.legs) {
                var leg = this.legs[i];
                var next = leg.next_joint;
                while (next != null) {
                    next.setAngle(10);
                    next = next.next_joint;
                }
            }

        } else
        if (value == "stand") {
            for (var i in this.legs) {
                var leg = this.legs[i];
                var next = leg.next_joint;
                while (next != null) {
                    next.setAngle(45);
                    next = next.next_joint;
                }
            }
        }

        robot.showTransparent(false);
        var box = new THREE.Box3().setFromObject( robot );
        //robot.position.z = -box.min.z;

        //var helper = new THREE.BoxHelper(robot, 0x0000ff);
        //scene.add(helper);

        render();
    }

    check_legs(point) {
        var closest = null;
        var distance = 1000;
        for (var i in this.legs) {
            var leg = this.legs[i];
            var dist = leg.calculateDistanceToPoint(point);
            if (dist < distance) {
                distance = dist;
                closest = leg;
            }
        }

        console.log("-------------------------------------------");
        console.log(" Selected " + closest.name)
        robot.setOutline(false);

        for (var c in closest.outlines) {
            var outline = closest.outlines[c];
            outline.visible = true;
        }

        closest.solve_ik(point);

        return closest;
    }

};

var robot = new Robot();

console.log("Robot created " + robot.name);

function center_object(obj3d) {
    var box = new THREE.Box3().setFromObject( obj3d );
    box.getCenter( obj3d.position );
    obj3d.position.multiplyScalar( - 1 );
    return box;
}

function create_pivot_geometry(color, scale = 0.1) {
    //console.log("Pivot creation");
    var geometry = new THREE.SphereGeometry( PIVOT_SIZE, 8, 8 );
    var material = new THREE.MeshBasicMaterial( color );
    var sphere = new THREE.Mesh( geometry, material );
    sphere.scale.set(scale, scale, scale);
    return sphere;
}

function create_leg(angle) {
    var box;
    var size;
    var helper;
    var pivot;

    var empty_geometry = new THREE.Object3D();

    // rotateAboutPoint(mesh, new THREE.Vector3( 0, 0, 0 ), new THREE.Vector3( 1, 0, 0 ))
    // mesh.rotateOnAxis( new THREE.Vector3( 0, 1, 0 ), 90 * ( Math.PI/180 ) );
    // https://stackoverflow.com/questions/42812861/three-js-pivot-point/42866733#42866733

    var leg = new Leg(scene, "leg_root_" + angle, empty_geometry);

    // The process to create a leg is to create the joints
    // The joints will always be centered on the first pivot

    // The second pivot is defined by setNextPivotPosition and it
    // will define where the next joint will be attached.

    // Add the geometry centered on the joint attachment

    leg.setNextPivotPosition(42.3 * SCALE, 0,  3.8 * SCALE);

    //---------- Joint 1 -----------------
    var joint_coxa = new Joint(scene, "leg_coxa", joint_mesh_1);
    joint_coxa.setPivotVisibility(false);
    joint_coxa.setNextPivotPosition(12.5 * SCALE, 0, 0);
    joint_coxa.setOrientation('Z');
    leg.attach(joint_coxa);

    //---------- Joint 2 -----------------
    var joint_femur = new Joint(scene, "leg_femur", joint_mesh_2);
    joint_femur.setNextPivotPosition(30.3 * SCALE, 17.0 * SCALE, 0.5 * SCALE);
    joint_femur.setOrientation('Y');

    joint_coxa.attach(joint_femur);

    //---------- Joint 3 -----------------

    var joint_claws = new Joint(scene, "leg_claws", joint_mesh_3);
    joint_claws.setNextPivotPosition(92 * SCALE, -5.0 * SCALE, -3.5 * SCALE);
    joint_claws.setOrientation('Y');

    joint_femur.attach(joint_claws);

    joint_femur.post_init();
    joint_coxa.post_init();
    joint_claws.post_init();

    //---------- Rotate leg -----------------

    leg.outlines = new Array();
    leg.outlines.push(joint_coxa.outline);
    leg.outlines.push(joint_femur.outline);
    leg.outlines.push(joint_claws.outline);

    leg.rotate_joint_euler( 0,0, angle * ( Math.PI/180 ));

    leg.updateMatrixWorld();
    leg.calculateTotalLegLength();

    //console.log(" TOTAL DISTANCE LEG " + leg.total_length);

    return leg;
}

function finished_loading() {
    console.log("- Mesh loaded - " + loaded_meshes);
    loaded_meshes++;

	robot.state = 'Loading of ' + loaded_meshes + "/" + total_meshes;

    if (loaded_meshes != total_meshes)
        return;

    console.log("--------------------------------------- ");
    console.log("- Create legs - " + loaded_meshes);

    var group = new THREE.Group();
    robot.add( group );
    robot.shell = group;

    group.add(shell);

	robot.position.z += 10 * SCALE;

    robot.legs[0] = create_leg(0);
    robot.legs[1] = create_leg(90);
    robot.legs[2] = create_leg(180);
    robot.legs[3] = create_leg(270);

    for( x=0 ;x<4 ;x++ ) {
        robot.shell.add(robot.legs[x]);

        // Attach all the other legs to each other
        for( c=0 ;c<4 ;c++ ) {
            if (x != c)
                robot.legs[x].attach_root(robot.legs[c]);
        }
    }

    world.add(robot);

	robot.state = 'Loaded';

    //------------- GROUND PLANE -----------------------------------
    var geometry = new THREE.PlaneGeometry( 10, 10 );
    var material = new THREE.MeshPhongMaterial( {color: 0xffffff, side: THREE.DoubleSide, transparent: true, opacity: 0.3} );
    var plane = new THREE.Mesh( geometry, material );
    plane.visible = false;

    robot.ground_plane = plane;
    world.add( plane );

    scene.add(world);
    animate();
}

function init() {
    scene = new THREE.Scene();
    scene.add( new THREE.AmbientLight( 0x999999 ) );

    camera = new THREE.PerspectiveCamera( 35, window.innerWidth / window.innerHeight, 0.1, 100 );

    // Z is up for objects intended to be 3D printed.

    camera.up.set( 0, 0, 1 );
    camera.position.set( 0, -9, 6 );

    camera.add( new THREE.PointLight( 0xffffff, 0.8 ) );

    scene.add( camera );

    var grid = new THREE.GridHelper( 25, 50, 0xffffff, 0x555555 );
    grid.rotateOnAxis( new THREE.Vector3( 1, 0, 0 ), 90 * ( Math.PI/180 ) );
    scene.add( grid );

    renderer = new THREE.WebGLRenderer( { antialias: true } );
    renderer.setClearColor( 0x999999 );
    renderer.setPixelRatio( window.devicePixelRatio );
    renderer.setSize( window.innerWidth, window.innerHeight );
    document.body.appendChild( renderer.domElement );

    //------------- STL LOADER  ------------------------------------

    var loader = new THREE.STLLoader();

    var material_blue  = new THREE.MeshPhongMaterial( { color: 0x0e2045, specular: 0x111111, shininess: 200 } );
    var material_black = new THREE.MeshPhongMaterial( { color: 0x111111, specular: 0x111111, shininess: 200 } );
    var material_white = new THREE.MeshPhongMaterial( { color: 0xffffff, specular: 0x111111, shininess: 200 } );
    var material_transparent = new THREE.MeshLambertMaterial({color: 0x0e2045, transparent: true, opacity: 0.3});

    //----------------- SHELL ------------------------

    total_meshes++;
    loader.load( 'models/body.STL', function ( geometry ) {
        var mesh = new THREE.Mesh( geometry, material_blue.clone() );
        mesh.position.set( 0, 0, 0 );
        mesh.rotation.set( 0, 0, 0 );
        mesh.scale.set( SCALE, SCALE, SCALE );
        mesh.castShadow = true;
        mesh.receiveShadow = true;
        mesh.visible = false;
        mesh.userData = { transparent: false };

        var mesh_trans = new THREE.Mesh( geometry, material_transparent.clone() );
        mesh_trans.scale.set( SCALE, SCALE, SCALE );
        mesh_trans.userData = { transparent: true };

        box = center_object( mesh );
        box = center_object( mesh_trans );

        shell.add( mesh );
        shell.rotation.set(  90 * ( Math.PI/180 ), 45 * ( Math.PI/180 ), 0 );

        shell.add( mesh_trans );
        finished_loading();
    });

    //---------------- JOINT 3 ------------------------

    total_meshes++;
    loader.load( 'models/pata.STL', function ( geometry ) {
        var mesh = new THREE.Mesh( geometry, material_blue.clone() );

        mesh.position.set( 0, 0, 0 );
        mesh.rotation.set( 0, 0 * ( Math.PI/180 ), 0 );
        mesh.scale.set( SCALE, SCALE, SCALE );

        mesh.castShadow = true;
        mesh.receiveShadow = true;
        mesh.visible = false;
        mesh.userData = { transparent: false };

        joint_mesh_3.add( mesh );

        var mesh_trans = new THREE.Mesh( geometry, material_transparent.clone() );
        mesh_trans.scale.set( SCALE, SCALE, SCALE );
        mesh_trans.userData = { transparent: true };

        joint_mesh_3.add( mesh_trans );

        box = center_object( joint_mesh_3 );
        size = box.getSize( new THREE.Vector3() );

        joint_mesh_3.position.x += size.x /2 - 7.5 * SCALE;
	    joint_mesh_3.position.y -= 5.0 * SCALE;
        joint_mesh_3.position.z += 2.5 * SCALE;

        finished_loading();
    });

    //---------------- JOINT 2 ------------------------

    total_meshes++;
    loader.load( 'models/brazo.STL', function ( geometry ) {
        var mesh = new THREE.Mesh( geometry, material_blue.clone() );

        mesh.position.set( 0, 0, 0 );
        mesh.rotation.set( 0 , 0, 0);
        mesh.scale.set( SCALE, SCALE, SCALE );

        mesh.castShadow = true;
        mesh.receiveShadow = true;
        mesh.visible = false;
        mesh.userData = { transparent: false };

        joint_mesh_2.add( mesh );

        var mesh_trans = new THREE.Mesh( geometry, material_transparent.clone() );
        mesh_trans.scale.set( SCALE, SCALE, SCALE );
        mesh_trans.userData = { transparent: true };

        joint_mesh_2.add( mesh_trans );

        center_object( joint_mesh_2 );
        joint_mesh_2.position.x += 15.0 * SCALE;
        joint_mesh_2.position.y += 8.5 * SCALE;
        joint_mesh_2.position.z += 0.8 * SCALE;

        finished_loading();
    });

    //---------------- JOINT 1 ------------------------

    total_meshes++;
    loader.load( 'models/hombro.STL', function ( geometry ) {
        var mesh = new THREE.Mesh( geometry, material_black.clone() );

        mesh.position.set( 0, 0, 0 );
        mesh.rotation.set( 0, 0 * ( Math.PI/180 ), 0 );
        mesh.scale.set( SCALE, SCALE, SCALE );

        mesh.castShadow = true;
        mesh.receiveShadow = true;
        mesh.visible = false;
        mesh.userData = { transparent: false };

        joint_mesh_1.add( mesh );
        joint_mesh_1.obj_mesh = mesh_trans;

        var mesh_trans = new THREE.Mesh( geometry, material_transparent.clone() );
        mesh_trans.scale.set( SCALE, SCALE, SCALE );
        mesh_trans.userData = { transparent: true };

        joint_mesh_1.add( mesh_trans );

        center_object( joint_mesh_1 );
        joint_mesh_1.position.x += 6.2 * SCALE;

        finished_loading();
    });


    controls = new THREE.OrbitControls( camera, renderer.domElement );
    controls.addEventListener( 'change', render );
    controls.target.set( 0, 0.2, 0.0 );
    controls.update();
    window.addEventListener( 'resize', onWindowResize, false );
}

function onWindowResize() {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();

    renderer.setSize( window.innerWidth, window.innerHeight );

    render();
}

//-------------------------------------------
//---------- MOUSE FUNCTIONS ----------------
//-------------------------------------------

raycaster = new THREE.Raycaster();
mouse = new THREE.Vector2();
mouse_raw = new THREE.Vector2();
mouse_down = new THREE.Vector2();
INTERSECTED = new THREE.Object3D();
mouse_intersect = create_pivot_geometry({color: 0xffff00});

document.addEventListener('mousemove', onDocumentMouseMove, false);
document.addEventListener('mousedown', onMouseDown, false);
document.addEventListener('mouseup', onMouseUp, false);

function onDocumentMouseMove(event) {
    event.preventDefault();
    mouse_raw.x = event.clientX;
    mouse_raw.y = event.clientY;

    mouse.x = (event.clientX / window.innerWidth) * 2 - 1;
    mouse.y = -(event.clientY / window.innerHeight) * 2 + 1;
}

function onMouseDown(event){
    if (event.button == 0) {
        //console.log("Mouse DOWN position: (" + mouse.x + ", "+ mouse.y + ")");
        mouse_down.x = mouse_raw.x;
        mouse_down.y = mouse_raw.y;
    }
}

function onMouseUp(event){
    if (event.button == 0) {
        var x = mouse_down.x - mouse_raw.x;
        var y = mouse_down.y - mouse_raw.y;
        dist = Math.sqrt(x*x + y*y);
        //console.log("Mouse UP position: (" + mouse_raw.x + ", "+ mouse_raw.y + ")" + dist);
        if (dist < 2) {
            manageRaycasterIntersections(scene, camera);
        }
    }
}

function highlight(obj_mesh, value) {
    if (!obj_mesh || obj_mesh.material === undefined) {
        return;
    }

    if (value == true) {
        if (obj_mesh.material.emissive !== undefined) {
            obj_mesh.currentHex = obj_mesh.material.emissive.getHex();
            obj_mesh.material.emissive.setHex( 0x00ffff );
        }
        else {
            obj_mesh.currentHex = obj_mesh.material.color.getHex();
            obj_mesh.material.color.setHex(0x00ffff);
        }

    } else {
        if (obj_mesh.material.emissive !== undefined) {
            obj_mesh.material.emissive.setHex( obj_mesh.currentHex );
        }
        else {
            obj_mesh.material.color.setHex(obj_mesh.currentHex);
        }
    }

}

function manageRaycasterIntersections(scene, camera) {
    camera.updateMatrixWorld();

    raycaster.setFromCamera(mouse, camera);
    robot.ground_plane.visible = true;
    var intersects = raycaster.intersectObjects(world.children, true); //
    robot.ground_plane.visible = false;

    highlight(INTERSECTED, false);

    if (intersects.length > 0) {
        for (var x = 0; x < intersects.length; x++ ) {
            var intrs_obj = intersects[ x ].object;
            if (intrs_obj.ignore_collision === undefined) {
                p = intersects[x].point;
                console.log(" Impact " + p.x.toFixed(2) + "," + p.y.toFixed(2) + "," + p.z.toFixed(2));

                highlight(intrs_obj, true);

                INTERSECTED = intrs_obj;
                robot.check_legs(p);

                //mouse_intersect.position.set(p.x - PIVOT_SIZE /2 , p.y - PIVOT_SIZE / 2, p.z - PIVOT_SIZE / 2);

                mouse_intersect.position.set(p.x, p.y, p.z);
                break;
            } else {
                //console.log(" Ignored geometry ");
            }
        }
    }
}

init();

scene.add(mouse_intersect);

