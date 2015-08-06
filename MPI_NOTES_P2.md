Slides
======

- OpenMPI ==> diversas implementações
- Mesmo programa é executado em todos os processos
- Processo != Processador. Em OpenMP, existe um único processo. Em OpenMPI, existe o número de processos que você especificar em `mpirun -np <number> <program>`
- Naturalmente, se o número de processadores (`nproc`) for maior ou igual ao número de processos, então esses processos serão cada um executados em cada processador, muito provavelmente (dependendo do *scheduler* do sistema operacional).
- Usualmente, utilizar um número de processos maior que o de processadores disponíveis acaba piorando o desempenho. O número ótimo tende a ser igual ao mesmo número de processadores.

Template OpenMPI
================

#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(null, null) ou MPI_Init(&argc, &argv) // obrigatório, e preferencialmente deveria ser a primeira instrução
    MPI_Finalize // obrigatório! Faz parte da especificação. Se precisar dar exit(<number>) antes, chame MPI_Finalize() antes de fazer isso!
}

Em C, todas as funções são estilo:
MPI_<maiuscula><resto minusculo, mas com eventual CamelCaseSacou>: exemplos incluem MPI_Broadcast, MPI_AllGather

Usualmente fazemos isso aqui também:

int rank, nprocs;

MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

Não desperdice processamento, use o rank 0 também p/ cálculos!!

- Comunicadores: "tags", ou conjunto de processos. As tags do [dwm](http://dwm.suckless.org/) são uma ótima analogia, mas provavelmente nunca ninguém o usou mesmo. Pense em tags do wordpress, do delicious, ou, talvez melhor ainda, do gmail. Em particular, existe uma tag que está atribuída a todos os processo (como se fosse o "All mail" do Gmail): MPI_COMM_WORLD. Só que ninguém chama isso de tags, a nomenclatura correta é comunicador. É possível definir comunicadores customizados (exemplo: processos ímpares x processos pares), mas isso é avançado e provavelmente não vai cair.

Nosso template fica assim agora:


int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, nprocs;

    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Finalize();
}

Para medir tempo:

MPI_Wtime() --> retorna um double. Não importa o que isso representa, apenas que a unidade é segundos, só estamos interessamos na diferença entre tempos:

double start, end;
start = MPI_Wtime();
// heavy processing here...
end = MPI_Wtime();
printf("Tempo: %lf\n", end - start); --> representa o tempo em segundos.


Para compilar: usar mpicc em vez de gcc/clang/icc ou mpic++ em vez de g++/clang++/icpc. Apenas uma conveniência, mas teoricamente também é possível utilizar os outros compiladores, mas é preciso especificar os headers e as libraries corretas.


$ mpicc -O2 -Wall program.c -o program
$ mpirun -np 2 program <arg1> <arg2> ...

Troca de mensagens
==================

Todo envio precisa ter recebimento or ==> FAIL!
Síncrono (bloqueante) ou não
Peer-to-Peer (P2P) ou um-a-um


Bloqueante ==> espera até enviar os dados. "Prende" o programa. Cuidado com deadlock, principalmente se for algo circular...
Não-bloqueante ==> "event-driven". Registra o envio mas continua executando outras coisas. Parecido com Ajax nesse sentido.

Ordem e equidade:
-----------------

MPI garante a ordem que você vez.

MPI_Send(...) (1)
MPI_Send(...) (2)

Necessariamente (1) será executado antes de (2).

No entanto, ele não é *safe* em termos de equidade! Se, por exemplo, 0 e 1 enviam para 2, mas 2 só recebe uma vez, então dá problema! 2 deveria receber duas vezes nesse caso. Equidade = true <=> número de envios == número de recebimentos


Tipos de dados (index): <-- os mais importantes apenas
=======================

- MPI_CHAR
- MPI_INT
- MPI_FLOAT
- MPI_DOUBLE
- MPI_BYTE

COMUNICAÇÃO // THERE BE DRAGONS --> com exemplos
================================

MPI_Send(void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm);

Exemplo típico: enviar int v[10]; do rank 0 para o 1:

MPI_Send(v, 10, MPI_INT, 1, MPI_DUMMY_TAG, MPI_COMM_WORLD);

Rationale: não precisa usar &v, pois v já é ponteiro; a tag pode ser qualquer número positivo (na verdade, não-negativo). Eu gosto de fazer #define MPI_DUMMY_TAG 1 pra usar sempre a mesma. Não usar tags diferentes para não complicar! Não precisa dizer que o rank de "origem" é o 0, você só precisa garantir que isso seja executado apenas por ele, se é o que você quer. Na prática, ficaria assim:

if(rank == 0) {
    MPI_Send(v, 10, MPI_INT, 1, MPI_DUMMY_TAG, MPI_COMM_WORLD);
}

Se quisesse enviar apenas uma soma: double sum;:

MPI_Send(&sum, 1, MPI_DOUBLE, 1, MPI_DUMMY_TAG, MPI_COMM_WORLD);

O Recv é análogo...quase igual. Fazendo os dois exemplos:

MPI_Send(v, 10, MPI_INT, 1, MPI_DUMMY_TAG, MPI_COMM_WORLD);
MPI_Recv(v, 10, MPI_INT, 0, MPI_DUMMY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); <-- compare acima


MPI_Send(&sum, 1, MPI_DOUBLE, 1, MPI_DUMMY_TAG, MPI_COMM_WORLD);
MPI_Recv(&sum, 1, MPI_DOUBLE, 0, MPI_DUMMY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE); <-- compare de novo

Garanta que os dados existam em ambos os processos! Cuidado com escopos e ifs.

MPI_STATUS_IGNORE <== é, ninguém liga pro status. Se der erro, whatever, né?

Existe MPI_ANY_TAG para recebimento. Mas prefiro usar a DUMMY.
Também [existe](http://stackoverflow.com/questions/4348900/mpi-recv-from-an-unknown-source) MPI_ANY_SOURCE (descobri isso agora!).


Importante (extensão):
Envio não-bloqueante: I

MPI_Isend e MPI_Irecv. Parâmetro adicional: MPI_Request request.

--

MPI_Send(buf, count, datatype, dest, tag, comm)
MPI_Recv(buf, count, datatype, source, tag, comm, status)
MPI_Isend(buf, count, datatype, dest, tag, comm, request)
MPI_Irecv(buf, count, datatype, source, tag, comm, request)

Uso típico:

MPI_request ireq; (compare a diferença)

MPI_Send(&sum, 1, MPI_DOUBLE, 1, MPI_DUMMY_TAG, MPI_COMM_WORLD);
MPI_Isend(&sum, 1, MPI_DOUBLE, 1, MPI_DUMMY_TAG, MPI_COMM_WORLD, &ireq);

Recebimento:

MPI_Recv(&sum, 1, MPI_DOUBLE, 0, MPI_DUMMY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
MPI_IRecv(&sum, 1, MPI_DOUBLE, 0, MPI_DUMMY_TAG, MPI_COMM_WORLD, &ireq); // <== não existe STATUS aqui!! Status vira request.

Isso basta, mas, se quiser sincronizar...WAIT!

MPI_Wait(&ireq, &status); // bloca até a request do MPI_Isend associada a ireq for completada. Quase um "oncompleted" de Ajax/node.js.

Também tem MPI_Waitall(), mas não vai precisar usar isso, creio. 

Argumentos:
- http://www.mpich.org/static/docs/v3.1/www3/MPI_Waitall.html
- http://stackoverflow.com/questions/7149314/how-actually-mpi-waitall-works

Exemplo: MPI_Waitall(10, requests, statuses); // <== arrays. Melhor ignorar isso, realmente não deve cair. Só se lembrar do Wait() mesmo.

--------

MPI_SendRecv: combinação preguiçosa
