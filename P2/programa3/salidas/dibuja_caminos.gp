set xrange [-1:11]
set yrange [-1:11]
set pointsize 2
unset key
plot 'programa3_5.txt' with points, 'programa3_5.txt' with lines
set terminal pngcairo
set output './dibujos/camino_5.png'
replot
