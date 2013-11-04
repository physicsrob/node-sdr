define([], function() {
    
    function saveForm() {
        var config = {
            iq_input: $("#iq_input").val(),
            iq_output: $("#iq_output").val(),
            usb_input: $("#usb_input").val(),
            usb_output: $("#usb_output").val(),
            iq_tx_alpha: $("#iq_tx_alpha").val(),
            iq_tx_epsilon: $("#iq_tx_epsilon").val(),
            iq_rx_alpha: $("#iq_rx_alpha").val(),
            iq_rx_epsilon: $("#iq_rx_epsilon").val()
        };
        $.post("/config", config);
    }

    $(".slider").slider().on("slide", saveForm);
    $("select").change(saveForm);
});

