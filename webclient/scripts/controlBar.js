define([], function() {
    var currentMode = 'idle';
    var vfoFreq = 14000, tuneFreq=14010;
    function tune() {
        $.post("/tune", {vfo:vfoFreq, tune:tuneFreq}, function() {
        });
    }

    $("#modeIdle").parent().click(function(){
        currentMode='idle';
    });

    $("#modeRX").parent().click(function(){
        currentMode='RX';
    });

    $("#modeTX").parent().click(function(){
        currentMode='TX';
    });

    $("#vfo").change(function() {
        vfoFreq=$(this).val();
        tune();
    });

    $("#tune").change(function() {
        tuneFreq=$(this).val();
        tune();
    });

    setInterval(function() {
        $.post("/run", {mode:currentMode}, function() {
        });
    }, 100);

});

