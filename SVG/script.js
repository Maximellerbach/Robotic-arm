// <![CDATA[
    var xhr = new XMLHttpRequest(); 
    function btn(cmdSend) {
        xhr.open('GET', cmdSend + '&" + securityKey + "&_=' + Math.random());
        xhr.send(null);
    }
// ]]>