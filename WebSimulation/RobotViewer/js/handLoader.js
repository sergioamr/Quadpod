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

var Robot = function()  {
    this.name = 'Spider';
	this.state = 'Loading';
    this.display_outline = true;
    this.geometry = new THREE.Object3D();
    this.legs = new Array();

    this.setOutline = function(value) {
        for (var i in this.legs) {
            leg=this.legs[i];
            for (var c in leg.highlights) {
                outline = leg.highlights[c];
                outline.visible = value;
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

function create_leg(angle) {
    var box;
    var size;
    var helper;

    // rotateAboutPoint(mesh, new THREE.Vector3( 0, 0, 0 ), new THREE.Vector3( 1, 0, 0 ))
    // mesh.rotateOnAxis( new THREE.Vector3( 0, 1, 0 ), 90 * ( Math.PI/180 ) );
    // https://stackoverflow.com/questions/42812861/three-js-pivot-point/42866733#42866733

    var leg = new THREE.Object3D();

    var joint_body = new THREE.Object3D();
    leg.add(joint_body);

    //---------- Joint 1 -----------------
    var j1_clone = joint_mesh_1.clone();
    box = center_object( j1_clone );    
    j1_clone.position.x += 48.5 * SCALE;
	j1_clone.position.z += 3.8 * SCALE;

    //---------- Joint 2 -----------------
    var j2_clone = joint_mesh_2.clone();	
    center_object( j2_clone );
    j2_clone.position.x += 22.5 * SCALE;
	j2_clone.position.y += 8.0 * SCALE;
	j2_clone.position.z += 7.5 * SCALE;
	
    //---------- Joint 3 -----------------

    var j3_clone = joint_mesh_3.clone();
    box = center_object( j3_clone );
    size = box.getSize( new THREE.Vector3() );

    j3_clone.position.x += size.x /2 + 10 * SCALE;
	j3_clone.position.y += 3.0 * SCALE;
	j3_clone.position.z += 8.5 * SCALE;
	
    var j4_clone = joint_mesh_4.clone();
    size = box.getSize( new THREE.Vector3() );

    //---------- Rotate leg -----------------

    joint_body.add(j1_clone);

    leg.highlights = new Array();

    helper = new THREE.BoxHelper(j1_clone, 0x0000ff);
    leg.add(helper);
    leg.highlights.push(helper);

    j1_clone.add(j2_clone);

    helper = new THREE.BoxHelper(j2_clone, 0x0000ff);
    leg.add(helper);
    leg.highlights.push(helper);

    j2_clone.add(j3_clone);

    helper = new THREE.BoxHelper(j3_clone, 0x0000ff);
    leg.add(helper);
    leg.highlights.push(helper);

    j3_clone.add(j4_clone);

    helper = new THREE.BoxHelper(j4_clone, 0x0000ff);
    leg.add(helper);
    leg.highlights.push(helper);

    leg.rotation.set( 0,0, angle * ( Math.PI/180 ));

    return leg;
}

var loaded_meshes = 0;
var total_meshes = 0;

function finished_loading() {
    console.log("- Mesh loaded - " + loaded_meshes);
    loaded_meshes++;
	
	robot.state = 'Loading of ' + loaded_meshes + "/" + total_meshes;

    if (loaded_meshes != total_meshes)
        return;

    console.log("--------------------------------------- ");
    console.log("- Create legs - " + loaded_meshes);

    robot.geometry.add( shell );
	robot.geometry.position.z += 10 * SCALE;

    robot.legs[0] = create_leg(0);
    robot.legs[1] = create_leg(90);
    robot.legs[2] = create_leg(180);
    robot.legs[3] = create_leg(270);

    for( x=0 ;x<4 ;x++ )
        robot.geometry.add(robot.legs[x]);

    scene.add(robot.geometry);
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

        box = center_object( mesh );

        var dummyshell = new THREE.Object3D();
        dummyshell.add( mesh );
        dummyshell.rotation.set(  90 * ( Math.PI/180 ), 45 * ( Math.PI/180 ), 0 );        

        shell.add(dummyshell);
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

        joint_mesh_3.add( mesh );
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

        joint_mesh_2.add( mesh );
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

        joint_mesh_1.add( mesh );
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
