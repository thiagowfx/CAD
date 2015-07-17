# plot graphics for ex2
library(ggplot2)

x <- c(1, 2, 4, 8)
ys <- c(0.0001286, 0.0003367, 0.0011592, NA)
ym <- c(0.0030924, 0.0275752, 0.0606025, 0.1234369)
yl <- c(0.0069657, 0.0928792, 0.2353698, 0.4692058)

png(file="ex2_mpi.png", bg="transparent")

g <- ggplot() + xlab("Número de processos") + ylab("Tempo (s)") + ggtitle("Produto de Matrizes (versão MPI)") + labs(colour="Tamanho da matriz")

g <- g + geom_point(aes(x = x, y = ys), size = 3) + geom_line(aes(x = x, y = ys, colour="small"))

g <- g + geom_point(aes(x = x, y = ym), size = 3) + geom_line(aes(x = x, y = ym, colour="medium"))

g <- g + geom_point(aes(x = x, y = yl), size = 3) + geom_line(aes(x = x, y = yl, colour="large"))

g

dev.off()
