/** LUEGENDETEKTOR v0.0.
 *
 * In dieser Datei findet ihr den Code fuer den Luegendetektor aus der Folge
 * "Luegendetektor" http://youtube.com/philsphysics
 * Haftung meinerseits ist ausgeschlossen.
 * Philip Haeusser, 13. Juli 2017
 * phil@phils-physics.de
 * /
 */

// Wir laden die Funktionen vom KosmosBits, damit wir zum Beispiel die RGB LED
// nutzen können; Wenn ihr einen normalen Arduino verwendet, braucht ihr das
// nicht machen. In diesem Fall braucht ihr ggf. andere header.
#include <Adafruit_NeoPixel.h>
#include <KosmoBits_Pins.h>
#include <KosmoBits_Pixel.h>

KosmoBits_Pixel pixel;

const float moving_avg_weight = 0.8;
float moving_avg = 0.0;
int counter = 0;

const int AUS = 0;
const int ROT = 1;
const int GELB = 2;
const int GRUEN = 3;
const int WEISS = 4;

int mittelwert = 10;

void set_pixel(int color) {
  switch (color) {
    case AUS:
      pixel.setColor(0, 0, 0, 0);
      break;
    case ROT:
      pixel.setColor(255, 0, 0, 50);
      break;
    case GELB:
      pixel.setColor(255, 255, 0, 50);
      break;
    case GRUEN:
      pixel.setColor(0, 255, 0, 50);
      break;
    case WEISS:
      pixel.setColor(255, 255, 255, 50);
      break;
    default:
      pixel.setColor(0, 0, 0, 0);
      break;
  }
}

int kalibriere(int sekunden) {  // Wir kalibrieren die Messung, damit wir die
                                // Veränderungen der Leitfähigkeit gut sehen
                                // können, denn darauf kommt es ja an - auf die
                                // Veränderungen
  int messungen_pro_sekunde = 10;
  int num_messungen = messungen_pro_sekunde * sekunden;
  double sum_werte = 0;

  for (int i = 0; i < num_messungen; ++i) {
    set_pixel(i % 2);
    sum_werte += analogRead(A0);
    delay(int(1000 / messungen_pro_sekunde));
  }

  return int(sum_werte / num_messungen);
}

void setup() {
  pinMode(A0, INPUT);
  Serial.begin(115200);
  Serial.println("Kalibriere");
  mittelwert = kalibriere(5);
}

void loop() {
  float reading = analogRead(A0) - mittelwert;
  reading = (5. * reading) / 1.024;

  if (counter % 10 == 0) {
    Serial.print(reading);
    Serial.println(" mV");
  }

  counter++;
  delay(10);
}
