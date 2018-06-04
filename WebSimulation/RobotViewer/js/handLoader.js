if ( ! Detector.webgl ) Detector.addGetWebGLMessage();

var camera, scene, renderer;

var SCALE = 0.01

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

var robot = new THREE.Object3D();
var shell = new THREE.Object3D();
var pelvis = new THREE.Object3D();
var joint_mesh_1 = new THREE.Object3D();
var joint_mesh_2 = new THREE.Object3D();
var joint_mesh_3 = new THREE.Object3D();
var joint_mesh_4 = new THREE.Object3D();

function create_leg(angle) {
    // rotateAboutPoint(mesh, new THREE.Vector3( 0, 0, 0 ), new THREE.Vector3( 1, 0, 0 ))
    // mesh.rotateOnAxis( new THREE.Vector3( 0, 1, 0 ), 90 * ( Math.PI/180 ) );
    // https://stackoverflow.com/questions/42812861/three-js-pivot-point/42866733#42866733

    var leg = new THREE.Object3D();

    var joint_body = new THREE.Object3D();
    leg.add(joint_body);

    var clone = pelvis.clone();
    clone.rotation.set( 0,0, angle * ( Math.PI/180 ));
    shell.add(clone);

    //---------- Joint 1 -----------------
    var j1_clone = joint_mesh_1.clone();
    var box = new THREE.Box3().setFromObject( j1_clone );
    box.getCenter( j1_clone.position );
    j1_clone.position.multiplyScalar( - 1 );

    var size = box.getSize( new THREE.Vector3() );

    j1_clone.position.x += 50.0 * SCALE;
    joint_body.add(j1_clone);

    //---------- Joint 2 -----------------
    var j2_clone = joint_mesh_2.clone();

    var box = new THREE.Box3().setFromObject( j2_clone );
    box.getCenter( j2_clone.position );
    j2_clone.position.multiplyScalar( - 1 );

    j2_clone.position.x +=  j1_clone.position.x + 19 * SCALE;
    j2_clone.position.z +=  j1_clone.position.y + 32 * SCALE;

    j1_clone.add(j2_clone);

    //---------- Joint 3 -----------------

    leg.rotation.set( 0,0, angle * ( Math.PI/180 ));

    return leg;
}

var loaded_meshes = 0;
var legs = new Array();

function finished_loading() {
    console.log("- Mesh loaded -");
    loaded_meshes++;

    if (loaded_meshes < 5)
        return;

    console.log("- Create legs -");

    robot.add( shell );

    legs[0] = create_leg(0);
    legs[1] = create_leg(90);
    legs[2] = create_leg(180);
    legs[3] = create_leg(270);

    for( x=0 ;x<4 ;x++ )
        robot.add(legs[x]);

    scene.add(robot);

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
    var material_shell = new THREE.MeshLambertMaterial({color: 0xffffff, transparent: true, opacity: 0.3});

    //----------------- SHELL ------------------------

    loader.load( 'models/Shell (x1).stl', function ( geometry ) {
        var mesh = new THREE.Mesh( geometry, material_shell );

        mesh.position.set( 0, 0, 0 );
        mesh.rotation.set( 0, 0, 0 );
        mesh.scale.set( SCALE, SCALE, SCALE );

        mesh.castShadow = true;
        mesh.receiveShadow = true;

        var box = new THREE.Box3().setFromObject( mesh );
        box.getCenter( mesh.position ); // this re-sets the mesh position
        mesh.position.multiplyScalar( - 1 );
        mesh.position.y /= 2;

        var dummyshell = new THREE.Object3D();
        dummyshell.add( mesh );
        dummyshell.rotation.set(  90 * ( Math.PI/180 ), 0, 0 );
        dummyshell.position.z+= 20 * SCALE;

        shell.add(dummyshell);

        finished_loading();
    });

    //---------------- JOINT 2 ------------------------

    loader.load( 'models/leg_bottom1.stl', function ( geometry ) {
        var mesh = new THREE.Mesh( geometry, material_white );

        mesh.position.set( 0, 0, 0 );
        mesh.rotation.set( 0, 180 * ( Math.PI/180 ), 0 );
        mesh.scale.set( SCALE, SCALE, SCALE );

        mesh.castShadow = true;
        mesh.receiveShadow = true;

        joint_mesh_2.add( mesh );
        console.log("Joint Mesh 2 loaded 1");
        finished_loading();
    });

    loader.load( 'models/leg_top1.stl', function ( geometry ) {
        var mesh = new THREE.Mesh( geometry, material_white );

        mesh.position.set( 0, 0, 0 );
        mesh.rotation.set( 0, 180 * ( Math.PI/180 ), 0 );
        mesh.scale.set( SCALE, SCALE, SCALE );

        mesh.castShadow = true;
        mesh.receiveShadow = true;

        joint_mesh_2.add( mesh );
        console.log("Joint Mesh 2 loaded 2");
        finished_loading();
    });

    //---------------- JOINT 1 ------------------------

    loader.load( 'models/Hip Ball (x4).stl', function ( geometry ) {
        var mesh = new THREE.Mesh( geometry, material_black );

        mesh.position.set( 0, 0, 0 );
        mesh.rotation.set( 90 * ( Math.PI/180 ), 0, 0 );
        mesh.scale.set( SCALE, SCALE, SCALE );

        mesh.castShadow = true;
        mesh.receiveShadow = true;

        joint_mesh_1.add( mesh );
        console.log("Joint Mesh 1 loaded 1");
        finished_loading();
    });

    loader.load( 'models/Hip Servo (x4).stl', function ( geometry ) {
        var mesh = new THREE.Mesh( geometry, material_black );

        mesh.position.set( 0, -26.2 * SCALE, 35 * SCALE );
        mesh.rotation.set( 90 * ( Math.PI/180 ), 0, 0 );
        mesh.scale.set( SCALE, SCALE, SCALE );

        mesh.castShadow = true;
        mesh.receiveShadow = true;

        joint_mesh_1.add( mesh );
        console.log("Joint Mesh 1 loaded 2");
        finished_loading();
    });

    //---------------- PELVIS ------------------------

    loader.load( 'models/Pelvis(x4).stl', function ( geometry ) {
        var mesh = new THREE.Mesh( geometry, material_shell );

        mesh.position.set( 0, 0, 0 );
        mesh.rotation.set( 90 * ( Math.PI/180 ), 0, 0 );
        mesh.scale.set( SCALE, SCALE, SCALE );

        mesh.castShadow = true;
        mesh.receiveShadow = true;

        var box = new THREE.Box3().setFromObject( mesh );
        box.getCenter( mesh.position );
        mesh.position.multiplyScalar( - 1 );
        mesh.position.x += 35 * SCALE;

        pelvis.add( mesh );
        console.log("Link Body loaded");
        finished_loading();
    });

    var controls = new THREE.OrbitControls( camera, renderer.domElement );
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
