set xlabel "Tamano"
set ylabel "Tiempo"
unset key
plot "programa3" with points
set terminal pngcairo
set output "programa3.png"
replot

f(x)=a*x*log(x)*log(x)+b
fit f(x) "programa3" via a,b
plot "programa3" with points, f(x)
set terminal pngcairo
set output "programa3_ajuste.png"
replot

system("mv fit.log programa3_ajuste.log")
