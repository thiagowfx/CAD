# plot graphics for exercise #1

x <- c(1, 2, 4, 8)
y50 <- c(3.13, 3.76, 5.45, 15.88)
y100 <- c(28.35, 28.51, 34.29, 55.38)
png(file="jacobi.png", bg="transparent")
par(mfrow=c(1,2))
plot(x,  y50, main="Jacobi (n = 50)",  xlab="Número de threads", ylab="tempo(ms)", type="b", col="dark red", lwd=3)
plot(x, y100, main="Jacobi (n = 100)", xlab="Número de threads", ylab="tempo(ms)", type="b", col="blue", lwd=3)
dev.off()
