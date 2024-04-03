set xlabel "Tamano"
set ylabel "Tiempo"
unset key
plot "programa1" with points
set terminal pngcairo
set output "programa1.png"
replot

f(x) = a*x*log(x) + b
fit f(x) "programa1" via a,b
plot "programa1" with points, f(x)
set terminal pngcairo
set output "programa1_ajuste.png"
replot

system("mv fit.log programa1_ajuste.log")
