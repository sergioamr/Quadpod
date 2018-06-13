//####################################
//# InterfaceCode GUI
//####################################

class InterfaceCode {
    constructor() {
        this.speed = 0.8;
        this.message = ''

        this.robot_x = 0;
        this.robot_y = 0;
        this.robot_z = 0;
        this.automove = false;

        this.j1_x = 0;
        this.j1_y = 0;
        this.j1_z = 0;
        this.j1_highlight = false;

        this.j2_x = 0;
        this.j2_y = 0;
        this.j2_z = 0;
        this.j2_highlight = false;

        this.j3_x = 0;
        this.j3_y = 0;
        this.j3_z = 0;
        this.j3_highlight = false;

        this.j4_x = 0;
        this.j4_y = 0;
        this.j4_z = 0;
        this.j4_highlight = false;

        this.pose = "calibration";
    }

    test_function() {
        console.log("Test");
    };
};

window.onload = function() {
    var text = new InterfaceCode();
    var gui = new dat.GUI();

    gui.remember(text);

    gui.add(robot, 'name');

    var cam_f = gui.addFolder('camera');
    cam_f.add(controls.target, 'x', -1,1).step(0.01).onFinishChange(function(){ controls.update(); }).listen();
    cam_f.add(controls.target, 'y', -1,1).step(0.01).onFinishChange(function(){ controls.update(); }).listen();
    cam_f.add(controls.target, 'z', -1,1).step(0.01).onFinishChange(function(){ controls.update(); }).listen();

    cam_f.add(controls, 'zoom0', -1,1).step(0.01).onFinishChange(function(){ controls.update(); }).listen();

    gui.add(text, 'speed', 0, 1);

    gui.add(robot, 'display_outline').onFinishChange(
    function(value){
        if (value == true)
            console.log("enable highlight");
        else
            console.log("disable highlight");

        robot.setOutline(value);
    });

    gui.add(robot, 'transparent').onFinishChange(
        function(value){
            if (value == true)
                console.log("show transparent");
            else
                console.log("hide transparent");

            robot.showTransparent(value);
        }).listen();

    gui.add(robot, 'show_joints').onFinishChange(
        function(value){
            if (value == true)
                console.log("show joint");
            else
                console.log("hide joints");

            robot.showJoints(value);
        });

    gui.add(text, 'test_function');

    var folder = gui.addFolder('joints');

    var f1 = folder.addFolder('position');
    controller_x = f1.add(text, 'robot_x', 0, 1);
    f1.add(text, 'robot_y', 0, 1);
    f1.add(text, 'robot_z', 0, 1);
    f1.add(text, 'automove');

    controller_x.onChange(function(value) {
    // Fires on every change, drag, keypress, etc.
    });

    controller_x.onFinishChange(function(value) {
        // Fires when a controller loses focus.
        alert("The new value is " + value);
    });

    var f2 = folder.addFolder('joint 1');
    f2.add(text, 'j1_x', 0, 1);
    f2.add(text, 'j1_y', 0, 1);
    f2.add(text, 'j1_z', 0, 1);
    f2.add(text, 'j1_highlight').onFinishChange(
        function(){
            console.log("disable highlight");
        });

    var f3 = folder.addFolder('joint 2');
    f3.add(text, 'j2_x', 0, 1);
    f3.add(text, 'j2_y', 0, 1);
    f3.add(text, 'j2_z', 0, 1);
    f3.add(text, 'j2_highlight');

    var f3 = folder.addFolder('joint 3');
    f3.add(text, 'j3_x', 0, 1);
    f3.add(text, 'j3_y', 0, 1);
    f3.add(text, 'j3_z', 0, 1);
    f3.add(text, 'j3_highlight');

    var f4 = folder.addFolder('joint 4');
    f4.add(text, 'j4_x', 0, 1);
    f4.add(text, 'j4_y', 0, 1);
    f4.add(text, 'j4_z', 0, 1);
    f4.add(text, 'j4_highlight');

    gui.add(text, 'pose', [ 'calibration', 'relax', 'stand', 'extended', 'test' ] ).onFinishChange(function(value) {
        robot.setPose(value);
    });

    gui.add(robot, 'state').listen();

    var update = function() {
    //requestAnimationFrame(update);
    //console.log("Update");
    };

    finished_loading();
};
