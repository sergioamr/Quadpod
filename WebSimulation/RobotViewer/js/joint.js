//####################################
//# Joint
//####################################

class Joint extends THREE.Object3D {
    constructor(name, geometry) {
        super();
        this.name = name;
        this.next = new Array();
        this.init();

        var geo = geometry.clone();
        this.add(geo);
        this.obj = geo;
        this.createOutline();
        this.next_joint = null;

        this.orientation = 'Y';
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

