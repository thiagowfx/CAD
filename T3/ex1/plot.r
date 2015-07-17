# plot graphics for ex1

x <- c(1, 2, 3, 4, 5, 6, 7, 8)
y <- c(0.3234661, 0.3424842, 0.4510012, 0.6322366, 0.8157318, 0.9537072, 1.1293916, 1.2629257)

png(file="ex1.png", bg="transparent")

plot(x,  y, main="Produto Escalar",  xlab="Número de processos", ylab="Tempo (s)", type="b", col="dark red", lwd=3)

dev.off()
