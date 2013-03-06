var header = "<div id='wrapper' class='body'><div id='loginPage'><div class='ui-widget ui-corner-all ui-widget-content'><div class='ui-widget ui-corner-all ui-widget-header' style='height: 30px;'><div class='ui-datepicker-month'>Login</div></div><div style='padding-bottom: 13px; padding-left: 18px; padding-right: 18px; padding-top: 13px;'><input type='password' id='pwdInput'></input><br /><br /><button id='loginSubmit' onclick='javascript:login();'>Authenticate</button><br /><br /><div id='loginCounter' class='ui-widget' style='display: none'><div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'><p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span><strong>Door Partially Open!</strong></p></div></div></div></div></div><div id='bodyPage' style='display: none'><button id='refresh' onclick='javascript:status();'>Refresh</button><br /><br /><div id='tabs'><ul><li><a href='#tabs-1'>Controls</a></li></ul><div id='tabs-1'>";
var door1Content = "<br /><button style='width: 145px; height: 40px;' id='opendoor' onclick='javascript:OpenDoor();'>Open Door</button><br /><button style='width: 145px; height: 40px; margin-top: 4px;' id='closedoor' onclick='javascript:CloseDoor();'>Close Door</button><br /><button style='width: 145px; height: 40px; margin-top: 4px;' id='actuate' onclick='javascript:ActuateDoor();'>Actuate Door</button><br /><br /><hr /><br /><div id='leftopen' onmousedown='javascript:status();' class='ui-widget' style='display: none'><div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'><p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span><strong>Door Open!</strong></p></div></div><div id='leftclosed' onmousedown='javascript:status();' class='ui-widget' style='display: none'><div class='ui-state-highlight ui-corner-all' style='padding: 0 .7em;'><p><span class='ui-icon ui-icon-info' style='float: left; margin-right: .3em;'></span><strong>Door Closed.</strong></p></div></div><div id='leftunknown' onmousedown='javascript:status();' class='ui-widget' style='display: none'><div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'><p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span><strong>Door Partially Open!</strong></p></div></div>"
var door2Content = "<br /><button style='width: 129px; height: 40px; display: inline' id='opendoor' onclick='javascript:OpenDoor();'>Open Left</button><button style='width: 129px; height: 40px; display: inline' id='opendoor2' onclick='javascript:OpenDoor2();'>Open Right</button><br /><button style='width: 129px; height: 40px; margin-top: 4px; display: inline' id='closedoor'onclick='javascript:CloseDoor();'>Close Left</button><button style='width: 129px; height: 40px; margin-top: 4px; display: inline' id='closedoor2'onclick='javascript:CloseDoor2();'>Close Right</button><br /><button style='width: 129px; height: 40px; margin-top: 4px; display: inline' id='actuate'onclick='javascript:ActuateDoor();'>Run Left</button><button style='width: 129px; height: 40px; margin-top: 4px; display: inline' id='actuate2'onclick='javascript:ActuateDoor2();'>Run Right</button><br /><br /><hr /><br /><div id='leftopen' onmousedown='javascript:status();' class='ui-widget' style='display: none'><div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'><p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span><strong>Left Door Open!</strong></p></div></div><div id='leftclosed' onmousedown='javascript:status();' class='ui-widget' style='display: none'><div class='ui-state-highlight ui-corner-all' style='padding: 0 .7em;'><p><span class='ui-icon ui-icon-info' style='float: left; margin-right: .3em;'></span><strong>Left Door Closed.</strong></p></div></div><div id='leftunknown' onmousedown='javascript:status();' class='ui-widget' style='display: none'><div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'><p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span><strong>Left Door Partially Open!</strong></p></div></div><br /><div id='rightopen' onmousedown='javascript:status();' class='ui-widget' style='display: none'><div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'><p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span><strong>Right Door Open!</strong></p></div></div><div id='rightclosed' onmousedown='javascript:status();' class='ui-widget' style='display: none'><div class='ui-state-highlight ui-corner-all' style='padding: 0 .7em;'><p><span class='ui-icon ui-icon-info' style='float: left; margin-right: .3em;'></span><strong>Right Door Closed.</strong></p></div></div><div id='rightunknown' onmousedown='javascript:status();' class='ui-widget' style='display: none'><div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'><p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span><strong>Right Door Partially Open!</strong></p></div></div>"
var footer = "</div></div></div></div>";
var pwd = '';

$(function () {
var content = "<div id='wrapper' class='body'><div class='ui-widget ui-corner-all ui-widget-content'><div class='ui-widget ui-corner-all ui-widget-header' style='height: 30px;'><div class='ui-datepicker-month'>Error</div></div><div style='padding-bottom: 13px; padding-left: 18px; padding-right: 18px; padding-top: 13px;'><div id='loginCounter' class='ui-widget'><div class='ui-state-error ui-corner-all' style='padding: 0 .7em;'><p><span class='ui-icon ui-icon-alert' style='float: left; margin-right: .3em;'></span><strong>Arduino Config Error!</strong></p></div></div></div></div></div>";
if (doors == 1)
{
content = header + door1Content + footer;
} else if (doors == 2)
{
content = header + door2Content + footer;
}
$('#page').append(content);
$('button').button(); 
$('#tabs').tabs();
$('#pwdInput').bind('keypress', function (e) {
    var code = (e.keyCode ? e.keyCode : e.which);
    if (code == 13) { //Enter keycode
        login();
    }
});
showlogin(); }); 

function login() {
	$('#loginSubmit').attr('disabled','disabled').hide();
    //encode();
    //$('#pwdInput').val('');
    //$.post('/door', {
    //    cmd: '8',
    //    pwd: pwd
    //}, function (data) {
    //    passfail(data);
    //});
}

function passfail(xml) {
    var pass = 'test';
    var count = 'test';
    $(xml).find('auth').each(function () {
        pass = $(this).text();
    });
    $(xml).find('count').each(function () {
        count = $(this).text();
    });
    if (pass == 1) {
        $('#bodyPage').show();
        $('#loginPage').hide();
        status();
    } else {
        $('#loginCounter').show();
        if (count != 'Locked Out') {
            $('#loginCounter strong').text('Login failed!  Used ' + count + ' of 5 attempts.');
        } else {
            $('#loginCounter strong').text(count + '!');
        }
    }
}

function showlogin() {
    $('#loginPage').show();
    $('#pwdInput').focus();
}

function encode() {
    var input = $('#pwdInput').val();
    var _keyStr = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=';
    var output = '';
    var chr1, chr2, chr3, enc1, enc2, enc3, enc4;
    var i = 0;
    while (i < input.length) {
        chr1 = input.charCodeAt(i++);
        chr2 = input.charCodeAt(i++);
        chr3 = input.charCodeAt(i++);
        enc1 = chr1 >> 2;
        enc2 = ((chr1 & 3) << 4) | (chr2 >> 4);
        enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);
        enc4 = chr3 & 63;
        if (isNaN(chr2)) {
            enc3 = enc4 = 64;
        } else if (isNaN(chr3)) {
            enc4 = 64;
        }
        output = output + _keyStr.charAt(enc1) + _keyStr.charAt(enc2) + _keyStr.charAt(enc3) + _keyStr.charAt(enc4);
    }
    pwd = output;
}

function process(xml) {
    $('#leftopen').hide();
    $('#leftclosed').hide();
    $('#leftunknown').hide();
    $('#rightopen').hide();
    $('#rightclosed').hide();
    $('#rightunknown').hide();
    var leftstatus = 0;
    var rightstatus = 0;
    $(xml).find('left').each(function () {
        leftstatus = $(this).text();
    });
    $(xml).find('right').each(function () {
        rightstatus = $(this).text();
    });
    if (leftstatus == 'open') {
        $('#leftopen').show();
    }
    if (leftstatus == 'closed') {
        $('#leftclosed').show();
    }
    if (leftstatus == 'unknown') {
        $('#leftunknown').show();
    }
    if (rightstatus == 'open') {
        $('#rightopen').show();
    }
    if (rightstatus == 'closed') {
        $('#rightclosed').show();
    }
    if (rightstatus == 'unknown') {
        $('#rightunknown').show();
    }
}

function status() {
    window.scrollTo(0, 1);
    $.post('/door', {
        cmd: '7',
        pwd: pwd
    }, function (data) {
        process(data);
    });
}

function OpenDoor() {
    if (confirm('Open door?')) {
        $.post('/door', {
            cmd: '1',
            pwd: pwd
        });
    }
}

function CloseDoor() {
    if (confirm('Close door?')) {
        $.post('/door', {
            cmd: '2',
            pwd: pwd
        });
    }
}

function ActuateDoor() {
    if (confirm('Toggle door?')) {
        $.post('/door', {
            cmd: '3',
            pwd: pwd
        });
    }
}

function OpenDoor2() {
    if (confirm('Open door?')) {
        $.post('/door', {
            cmd: '4',
            pwd: pwd
        });
    }
}

function CloseDoor2() {
    if (confirm('Close door?')) {
        $.post('/door', {
            cmd: '5',
            pwd: pwd
        });
    }
}

function ActuateDoor2() {
    if (confirm('Toggle door?')) {
        $.post('/door', {
            cmd: '6',
            pwd: pwd
        });
    }
}