**Background**
<pre>
The Discrete Cosine Transform (DCT) is a mathematical transfomation that
transfrom a signal from spatial representation into frequency representation,
which is essential to the JPEG baseline compression process.

In an image, most of the energy will be concentrated in the lower frequencies. 
So, once an image is transformed into its frequency components, 
we can treat them selectively,
e.g. retaining lower frequency coefficients with high accuracy 
while squeezing the size of high frequency coefficients
</pre>

**Introduction**
<pre>
This program performs DCT on a 2D image and 
quantize its frequency coefficients.

For the enhanced features stated in the assignment, 
it is to implement the coefficient quantization.
</pre>

**Environment**
<pre>
visual studio 2019 C++ compiler
</pre>

**Command**
<pre>
C:\...\ dct img_path apply_quantization
i.e. : dct linda.bmp 0

apply_quantization specifies whether the obtained frequency coefficients need to be quantized,
where 1 means TRUE while 0 means FALSE
</pre>