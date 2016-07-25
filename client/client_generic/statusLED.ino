const uint16_t PixelCount = 1;
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266
const uint8_t AnimationChannels = 1; // we only need one as all the pixels are animated at once
NeoPixelBus<NeoGrbFeature, NeoEsp8266AsyncUart800KbpsMethod> pixels(PixelCount, PixelPin);  // this method uses pin 2 always
//NeoPixelAnimator animations(AnimationChannels);  // defaults to milliseconds
NeoPixelAnimator animations(PixelCount, NEO_MILLISECONDS);
// Possible values from 1 to 32768, and there some helpful constants defined as...
// NEO_MILLISECONDS        1    // ~65 seconds max duration, ms updates
// NEO_CENTISECONDS       10    // ~10.9 minutes max duration, centisecond updates
// NEO_DECISECONDS       100    // ~1.8 hours max duration, decisecond updates
// NEO_SECONDS          1000    // ~18.2 hours max duration, second updates
// NEO_DECASECONDS     10000    // ~7.5 days, 10 second updates
NeoGamma<NeoGammaTableMethod> colorGamma;


int hbState = 0;
void neoHeartBeat(const AnimationParam& param) {
  float hue = 200;  // the hue calculation in the library is janky (reversed and off by 100)
  RgbColor color = RgbColor(HslColor((360-hue+100)/360.0f, 0.5f, 0.25f));  // lum: 0 = dark, 0.25 = normal, 0.5 = bright
  //RgbColor color = HslColor(0.23f, 1.0f, 0.25f);
  RgbColor color1, color2, outColor;
  
  //float easing = NeoEase::ExponentialInOut(param.progress);
  float easing = NeoEase::QuadraticInOut(param.progress);
  
  if (hbState == 0) {
    color1 = color;
    color2 = RgbColor(0);
  }
  else if (hbState = 1) {
    color2 = color;
    color1 = RgbColor(0);
  }
  outColor = RgbColor::LinearBlend(color1, color2, easing);
  pixels.SetPixelColor(0, colorGamma.Correct(outColor));
}


void setupStatusLED() {
  pixels.Begin();
  pixels.Show();
}


void updateStatusLED() {
  if (animations.IsAnimating()) {
    animations.UpdateAnimations();
    pixels.Show();
  }
  else {
    animations.StartAnimation(0, 2000, neoHeartBeat);
    hbState = (hbState + 1) % 2;
  }
}

