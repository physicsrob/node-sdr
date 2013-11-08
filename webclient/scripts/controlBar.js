define([], function() {

    return function(controller) {
        $("#modeIdle").parent().click(function(){
            controller.changeMode('idle');
        });

        $("#modeRX").parent().click(function(){
            controller.changeMode('RX');
        });

        $("#modeTX").parent().click(function(){
            controller.changeMode('TX');
        });

        $("#vfo").change(function() {
            controller.changeVFO($(this).val());
        });

        $("#tune").change(function() {
            controller.changeTune($(this).val());
        });
    };


});

