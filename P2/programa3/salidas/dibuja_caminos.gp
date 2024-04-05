set xrange [-1:101]
set yrange [-1:101]
set pointsize 2
unset key
plot 'programa3_50.txt' with points, 'programa3_50.txt' with lines
set terminal pngcairo
set output './dibujos/camino_50.png'
replot
