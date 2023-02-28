$$
\left\{ 
\begin{array}{lc}
AddSubtractTypeTerm \leq AddSubtract.output \\\\
AddSubtract.output \leq SequenceScope.input \\\\
Constant.output \leq AddSubtract.in0 \\\\
Constant_1.output \leq AddSubtract.in1 \\\\
SequenceScope.input \leq scalar \\\\
Constant.value \leq Constant.output \\\\
Constant_1.value \leq Constant_1.output \\\\
......
\end{array}
\right.
$$
