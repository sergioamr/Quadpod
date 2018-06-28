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

        this.orientation = 'Y';
        this.counter = 0;
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
        var vector = new THREE.Vector3();
        vector.setFromMatrixPosition( this.matrixWorld );

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
        this.add(helper);
        this.outline = helper;
        this.outline.visible = false;
    }

    init() {
        this.pivot_prev = create_pivot_geometry({color: 0xffff00});
        this.pivot_next = create_pivot_geometry({color: 0xff0000});

        this.add(this.pivot_prev);
        this.add(this.pivot_next);

        this.pivot_next.visible = false;
        this.pivot_prev.visible = false;
        this.create_text();
    }

    setNextPivotPosition(x, y, z) {
        this.pivot_next.position.x = x;
        this.pivot_next.position.y = y;
        this.pivot_next.position.z = z;
    }

    setPivotVisibility(value) {
        this.pivot_next.visible = value;
        this.pivot_prev.visible = value;
    }

    attach(next_joint) {
        var pivot = this.pivot_next;
        next_joint.position.set(pivot.position.x, pivot.position.y, pivot.position.z);
        this.add(next_joint);
        this.next_joint = next_joint;
    }

    total_length() {

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
}

