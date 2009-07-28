document.write("<OBJECT ID='GameObject' CLASSID='CLSID:$guid0' WIDTH='$width' HEIGHT='$height' CODEBASE='$name.cab#Version=$version'>");
document.write("<PARAM NAME='MSN' VALUE='ON'>");
document.write("<PARAM NAME='AnonymousScores' VALUE='2'>");
document.write("<PARAM NAME='DownloadUrl' VALUE='http://zone.msn.com'>");
document.write("<PARAM name='Certificate' value='$axcert'>");
document.write("</OBJECT>");


function ProxyRestartGame()
{
	document.getElementById("GameObject").RestartGame();
}


function ProxyLoadGame()
{
	document.getElementById("GameObject").LoadGame();
}

var g_flashMovie = null;
