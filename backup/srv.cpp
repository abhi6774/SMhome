
// turns the GPIOs on and off
// if (header.indexOf("GET /5/on") >= 0)
// {
//   Serial.println("GPIO 5 on");
//   output5State = "on";
//   digitalWrite(output5, HIGH);
// }
// else if (header.indexOf("GET /5/off") >= 0)
// {
//   Serial.println("GPIO 5 off");
//   output5State = "off";
//   digitalWrite(output5, LOW);
// }
// else if (header.indexOf("GET /4/on") >= 0)
// {
//   Serial.println("GPIO 4 on");
//   output4State = "on";
//   digitalWrite(output4, HIGH);
// }
// else if (header.indexOf("GET /4/off") >= 0)
// {
//   Serial.println("GPIO 4 off");
//   output4State = "off";
//   digitalWrite(output4, LOW);
// }

// void drawGraph()
// {
//   String out;
//   out.reserve(2600);
//   char temp[70];
//   out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
//   out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
//   out += "<g stroke=\"black\">\n";
//   int y = rand() % 130;
//   for (int x = 10; x < 390; x += 10)
//   {
//     int y2 = rand() % 130;
//     sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
//     out += temp;
//     y = y2;
//   }
//   out += "</g>\n</svg>\n";

//   server.send(200, "image/svg+xml", out);
// }