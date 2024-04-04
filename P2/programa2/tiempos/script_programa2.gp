set xlabel "Tamano"
set ylabel "Tiempo"
unset key
plot "programa2" with points
set terminal pngcairo
set output "programa2.png"
replot

f(x) = a*x + b
fit f(x) "programa2" via a,b
plot "programa2" with points, f(x)
set terminal pngcairo
set output "programa2_ajuste.png"
replot

system("mv fit.log programa2_ajuste.log")
