set xlabel "Tamano"
set ylabel "Tiempo"

# Notacion cientifica y rotada
set format x "%.0e"
set xtics rotate by -45
set rmargin at screen 0.93

unset key # Para que no salga la leyenda

plot "programa1" with points
set terminal pngcairo
set output "programa1.png"
replot

f(x)=a*x*log(x)+b
#f(x)=a*x+b
fit f(x) "programa1" via a,b
plot "programa1" with points, f(x)
set terminal pngcairo
set output "programa1_ajuste.png"
replot

system("mv fit.log programa1_ajuste.log")
