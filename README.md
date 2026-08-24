# ProcessFlow

ProcessFlow é um projeto desenvolvido em **C** para gerenciamento e execução de tarefas e comandos utilizando recursos de processos do sistema operacional.

O projeto trabalha com conceitos como **processos, execução paralela, pipelines, processos em background e gerenciamento de tarefas**.

## Funcionalidades

* Execução de tarefas individuais
* Execução de tarefas em paralelo
* Execução de pipelines
* Execução de tarefas em background
* Gerenciamento de tarefas
* Processos utilizando `fork()` e `execvp()`
* Comunicação entre processos utilizando pipes

## Tecnologias

* **C**
* **GCC**
* **Linux / WSL**
* **Make**

## Estrutura

```text
processflow/
├── include/
│   ├── command.h
│   ├── exec.h
│   ├── jobs.h
│   ├── parser.h
│   ├── task.h
│   └── workflow.h
│
├── src/
│   ├── command.c
│   ├── exec.c
│   ├── interactive.c
│   ├── jobs.c
│   ├── main.c
│   ├── parser.c
│   ├── task.c
│   └── workflow.c
│
├── Makefile
└── README.md
```

## Compilação

Para compilar o projeto:

```bash
make
```

Também é possível compilar manualmente com:

```bash
gcc -Wall -Wextra -g -Iinclude -o src/processflow \
src/main.c src/interactive.c src/workflow.c src/parser.c \
src/task.c src/command.c src/exec.c src/jobs.c
```

## Execução

Após a compilação:

```bash
./src/processflow
```

## Objetivo

O projeto tem como objetivo aplicar, na prática, conceitos de **Sistemas Operacionais**, especialmente criação e gerenciamento de processos, execução de programas e comunicação entre processos.

## Autor

**Matheus Rocha**

GitHub: [@Mth71](https://github.com/Mth71)
