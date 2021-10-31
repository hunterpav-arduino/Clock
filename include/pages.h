// Upload request custom Web page
#include <Arduino.h>

static const char PAGE_CONFIG[] PROGMEM = R"(
{
  "uri": "/config",
  "title": "Config",
  "menu": true,
  "element": [
    { "name":"caption", "type":"ACText", "value":"<h2>Config:<h2>" },
    { "name":"name", "type":"ACInput", value: "", "label":"Name : " },
    { "name":"mqtt_server", "type":"ACInput", value: "", "label":"MQTT server : " },
    { "name":"fs_file", "type":"ACFile", "label":"Upload FS File (optional): ", "store":"fs" },
    { "name":"upload", "type":"ACSubmit", "value":"SAVE", "uri":"/config-save" }
  ]
}
)";

// Upload result display
static const char PAGE_CONFIG_SAVE[] PROGMEM = R"(
{
  "uri": "/config-save",
  "title": "Config",
  "menu": false,
  "element": [
    { "name":"caption", "type":"ACText", "value":"<h2>Config saved<h2>" },
    { "name":"files", "type":"ACText" }
  ]
}
)";

const char _PAGE_HOME[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta http-equiv="Content-type" content="text/html; charset=utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <script type="text/javascript" src="/dygraph.min.js"></script>
  <link rel="stylesheet" href="/dygraph.min.css" />
  <meta http-equiv="refresh" content="60">
</head>
<body>
<h1>{{CONFIG_NAME}} &ensp; {{AC_LINK}}</h1>
<br>

Uptime (s): {{UPTIME}} <br>

</body>
</html>
)rawliteral";
