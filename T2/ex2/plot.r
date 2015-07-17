# plot graphics for exercise #2

x_ms <- c(26, 27, 28)
y_ms <- c(17.446, 35.958, 73.986)
png(file="mergesort-serial.png", bg="transparent")
plot(x_ms, y_ms, main='Mergesort (serial)', xlab='k', ylab='tempo(s)', type='b', col='dark red', lwd=3)
dev.off()

x_mp <- c(26, 27, 28)
y_mp2 <- c(10.478, 19.763, 41.079)
y_mp4 <- c(10.341, 21.707, 40.459)
y_mp8 <- c( 9.981, 20.644, 42.551)
png(file="mergesort-parallel.png", bg="transparent")
plot(x_mp, y_mp2, main="Mergesort (paralelo)", xlab="k", ylab="tempo(s)", type='b', col='dark red', lwd=3)
lines(x_mp, y_mp4, type='b', col="green", lwd=3)
lines(x_mp, y_mp8, type='b', col="blue", lwd=3)
legend('topleft', c('2 threads', '4 threads', '8 threads'), lty=1, col=c('dark red', 'green', 'blue'))
dev.off()
