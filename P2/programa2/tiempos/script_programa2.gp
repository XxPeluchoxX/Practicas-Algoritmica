set xlabel "Tamano"
set ylabel "Tiempo"

# Notacion cientifica y rotada
set format x "%.0e"
set xtics rotate by -45
set rmargin at screen 0.93

unset key # Para que no salga la leyenda

plot "programa2" with points
set terminal pngcairo
set output "programa2.png"
replot

f(x) = a*x*x + b*x + c
fit f(x) "programa2" via a,b,c
plot "programa2" with points, f(x)
set terminal pngcairo
set output "programa2_ajuste.png"
replot

system("mv fit.log programa2_ajuste.log")
