# Regression

Identify the threshold voltage for the semiconductor device, from its I-V characteristics

Read the data from the datafile. <br>
Start with one curve (V<sub>g</sub>11), and find V<sub>th</sub>. You can use any suitable method. <br>
linear fits to the low and high voltage parts of the graph
start with some value of V<sub>th</sub> and try to minimize the RMS error from the linear fits by search for a better V<sub>th</sub>
fitting all the data to a model curve e.g.  I = I<sub>0</sub>(1-e<sup>-V/V<sub>0</sub></sup>)
Repeat the exercise for the different data columns in the datafile and finally plot Vth versus V<sub>g</sub>
