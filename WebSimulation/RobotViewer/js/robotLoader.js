if ( ! Detector.webgl ) Detector.addGetWebGLMessage();

var camera, scene, renderer;

var SCALE = 0.01
var controls;

function render() {
    renderer.render( scene, camera );
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

class Robot extends THREE.Object3D {
    constructor() {
        super();
        this.name = 'Spider';
        this.state = 'Loading';
        this.display_outline = false;
        this.show_joints = false;
        this.transparent = true;
        this.shell = null;
        this.legs = new Array();
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
        render();
    }

    showTransparent(value) {
        this.transparent = value;
        for (var i in this.shell.children ) {
            var mesh = this.shell.children[i];
            if (mesh.userData.transparent) {
                mesh.visible = value;
            } else {
                mesh.visible = !value;
            }
        }

        for (var i in this.legs) {
            var leg = this.legs[i];
            var next = leg.next_joint;

            while (next != null) {
                next.setTransparent(value);
                next = next.next_joint;
            }
        }
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

        render();
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

function create_pivot_geometry(color) {
    //console.log("Pivot creation");
    var geometry = new THREE.SphereGeometry( 2.5 * SCALE, 32, 32 );
    var material = new THREE.MeshBasicMaterial( color );
    var sphere = new THREE.Mesh( geometry, material );
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

    var leg = new Joint("leg_root", empty_geometry);

    // The process to create a leg is to create the joints
    // The joints will always be centered on the first pivot

    // The second pivot is defined by setNextPivotPosition and it
    // will define where the next joint will be attached.

    // Add the geometry centered on the joint attachment

    leg.setNextPivotPosition(42.3 * SCALE, 0,  3.8 * SCALE);

    //---------- Joint 1 -----------------
    var joint_coxa = new Joint("leg_coxa", joint_mesh_1);
    joint_coxa.setPivotVisibility(false);
    joint_coxa.setNextPivotPosition(12.5 * SCALE, 0, 0);
    joint_coxa.setOrientation('Z');
    leg.attach(joint_coxa);

    //---------- Joint 2 -----------------
    var joint_femur = new Joint("leg_femur", joint_mesh_2);
    joint_femur.setNextPivotPosition(30.3 * SCALE, 17.0 * SCALE, 0.5 * SCALE);
    joint_femur.setOrientation('Y');

    joint_coxa.attach(joint_femur);

    //---------- Joint 3 -----------------

    var joint_claws = new Joint("leg_claws", joint_mesh_3);
    joint_claws.setNextPivotPosition(92 * SCALE, -5.0 * SCALE, -3.5 * SCALE);
    joint_claws.setOrientation('Y');

    joint_femur.attach(joint_claws);

    //---------- Rotate leg -----------------

    leg.outlines = new Array();
    leg.outlines.push(joint_coxa.outline);
    leg.outlines.push(joint_femur.outline);
    leg.outlines.push(joint_claws.outline);

    leg.rotation.set( 0,0, angle * ( Math.PI/180 ));

    return leg;
}

var loaded_meshes = 0;
var total_meshes = 1;

function finished_loading() {
    console.log("- Mesh loaded - " + loaded_meshes);
    loaded_meshes++;

	robot.state = 'Loading of ' + loaded_meshes + "/" + total_meshes;

    if (loaded_meshes != total_meshes)
        return;

    console.log("--------------------------------------- ");
    console.log("- Create legs - " + loaded_meshes);

    robot.add( shell );
    robot.shell = shell;
	robot.position.z += 10 * SCALE;

    robot.legs[0] = create_leg(0);
    robot.legs[1] = create_leg(90);
    robot.legs[2] = create_leg(180);
    robot.legs[3] = create_leg(270);

    for( x=0 ;x<4 ;x++ )
        robot.add(robot.legs[x]);

    scene.add(robot);
	robot.state = 'Loaded';

    render();
}

function init() {
    scene = new THREE.Scene();
    scene.add( new THREE.AmbientLight( 0x999999 ) );

    camera = new THREE.PerspectiveCamera( 35, window.innerWidth / window.innerHeight, 1, 500 );

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

    var loader = new THREE.STLLoader();

    var material_blue  = new THREE.MeshPhongMaterial( { color: 0x0e2045, specular: 0x111111, shininess: 200 } );
    var material_black = new THREE.MeshPhongMaterial( { color: 0x111111, specular: 0x111111, shininess: 200 } );
    var material_white = new THREE.MeshPhongMaterial( { color: 0xffffff, specular: 0x111111, shininess: 200 } );
    var material_transparent = new THREE.MeshLambertMaterial({color: 0x0e2045, transparent: true, opacity: 0.3});

    //----------------- SHELL ------------------------

    total_meshes++;
    loader.load( 'models/body.STL', function ( geometry ) {
        var mesh = new THREE.Mesh( geometry, material_blue );
        mesh.position.set( 0, 0, 0 );
        mesh.rotation.set( 0, 0, 0 );
        mesh.scale.set( SCALE, SCALE, SCALE );
        mesh.castShadow = true;
        mesh.receiveShadow = true;
        mesh.visible = false;
        mesh.userData = { transparent: false };

        var mesh_trans = new THREE.Mesh( geometry, material_transparent );
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
        var mesh = new THREE.Mesh( geometry, material_blue );

        mesh.position.set( 0, 0, 0 );
        mesh.rotation.set( 0, 0 * ( Math.PI/180 ), 0 );
        mesh.scale.set( SCALE, SCALE, SCALE );

        mesh.castShadow = true;
        mesh.receiveShadow = true;
        mesh.visible = false;
        mesh.userData = { transparent: false };

        joint_mesh_3.add( mesh );

        var mesh_trans = new THREE.Mesh( geometry, material_transparent );
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
        var mesh = new THREE.Mesh( geometry, material_blue );

        mesh.position.set( 0, 0, 0 );
        mesh.rotation.set( 0 , 0, 0);
        mesh.scale.set( SCALE, SCALE, SCALE );

        mesh.castShadow = true;
        mesh.receiveShadow = true;
        mesh.visible = false;
        mesh.userData = { transparent: false };

        joint_mesh_2.add( mesh );

        var mesh_trans = new THREE.Mesh( geometry, material_transparent );
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
        var mesh = new THREE.Mesh( geometry, material_black );

        mesh.position.set( 0, 0, 0 );
        mesh.rotation.set( 0, 0 * ( Math.PI/180 ), 0 );
        mesh.scale.set( SCALE, SCALE, SCALE );

        mesh.castShadow = true;
        mesh.receiveShadow = true;
        mesh.visible = false;
        mesh.userData = { transparent: false };

        joint_mesh_1.add( mesh );
        joint_mesh_1.obj_mesh = mesh_trans;

        var mesh_trans = new THREE.Mesh( geometry, material_transparent );
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

init();
