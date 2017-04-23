# Honeywell Pressure Sensors

This Arduino library handles the [SSC][SSC] and [HSC][HSC] range of air pressure sensors from Honeywell with digital output only.

Product codes for supported parts will have S or 2–7 in the third-last position, e.g.: HSCDANN005PGSA5 is a 5V DIP part, single axial barbed port, 0–5PSI gauge pressure for dry gasses only, with SPI output.

The code is originally by Tomas Van Verrewegen <tomasvanverrewegen@telenet.be> https://code.google.com/p/arduino-ssc/

SPI functionality added by Alex Satrapa <grail@goldweb.com.au>.

# Sensors

Paraphrased from [the brochure][HSC-Datasheet]:

"The Honeywell TruStability® High Accuracy Silicon Ceramic (HSC) and Standard Accuracy Silicon Ceramic (SSC) air pressure sensors are piezoresistive silicon pressure sensors offering a digital output for reading pressure over the specified full scale pressure span and temperature range."

The SPI functionality in this code uses HSCDANN005PGSA5 as the normative reference, with Honeywell technical note [SPI Communication with Honeywell Digital Output Pressure Sensors](http://sensing.honeywell.com/spi-comms-digital-ouptu-pressure-sensors-tn-008202-3-en-final-30may12.pdf) as the technical reference.

[HSC]: http://sensing.honeywell.com/products/pressure-sensors-board-mount/low-pressure-sensors/TruStability-HSC/Ne/3025/N/3728 "Honeywell TruStability® HSC Series"
[HSC-Datasheet]: http://sensing.honeywell.com/index.php?ci_id=151133 "Honeywell TruStability® Board Mount Pressure Sensors: HSC Series — High Accuracy, Compensated/Amplified"
[SSC]: http://sensing.honeywell.com/honeywell-sensing-and-control-product-search?sid=14B66C2131E5&Ne=3025&N=3729 "Honeywell TruStability® SSC Series"
[SSC-Datasheet]: http://sensing.honeywell.com/honeywell-sensing-trustability-ssc-series-standard-accuracy-board-mount-pressure-sensors-50099533-a-en.pdf "Honeywell TruStability® Board Mount Pressure Sensors: SSC Series — Standard Accuracy, Compensated/Amplified"

# Usage

I plan to add usage examples to the wiki by the end of April 2017. Please contact me at grail@goldweb.com.au if you need assistance in using this library: your request for help helps motivate me to maintain the library!
