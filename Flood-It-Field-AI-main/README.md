
# Resolutor do jogo Flood Field (Inteligência Artifical)

## Aviso:
- Recomendo ler esse README.md usando uma ferramenta de leitura de MarkDown (https://stackedit.io/app#, por exemplo). Se desejar, também pode ler diretamente de meu github em https://github.com/RichardHeise/Flood-It-Field-AI, que estará público após o dia 15/08/2021 para evitar possíveis problemas com plágio. Boa leitura.

## Descrição:
 - Este trabalho se propõe a resolver tabuleiros do jogo Flood It (também conhecido como Flood Field ou Flood Fill) utilizando uma inteligência artifical programada em C++.

## O Problema
 - Como visto em aula, uma IA que resolve o Flood Field de maneira ótima -- isto é, sempre com a melhor sequência de jogadas -- pode ser obtida através de um algoritmo como o A*. No entanto, estamos limitados a 8GB de RAM e 2 minutos no máximo de execução de código; conclui-se, portanto, que seria inviável utilizar o A* e verificar todas as possibilidades de jogadas, ou seja, não há como garantir a solução ótima; temos, porém, como garantir uma solução boa o bastante: foi implementado um algoritmo guloso (greedy) que utiliza-se de uma boa heurística, previsão de jogadas e detecção de possíveis jogadas para chegar a uma solução razoável dentro de um tempo hábil.

## Código
 ### O código fonte possui três bibliotecas locais: matriz.h, IAra.h e floodlib.h. 
 - Em IAra.h (uma pequena piada com o nome hehe) você encontrará as funções referentes à IA em si.
 - Em matriz.h você encontrará funções referentes ao alocamento e controle de dados de um tabuleiro (matriz de char).
 - Em floodlib.h você encontrará funções referentes à implementação do jogo flood field.

 - Para mais informações aqui recomendo rodar o comando doxygen, que gerará uma documentação automática do código.
 ### Anatomia do código
 - IAra.cpp possui as implementações da IA.
 - matriz.cpp possui as implementações referentes a matrizes.
 - floodlib.cpp possui as implementações referentes ao jogo em si.
 - flood.cpp é somente nossa main.
 - Makefile é o arquivo de configuração make, para compilar o código.
 - README.md este arquivo.

## Fluxo
 - Em flood.cpp temos nossa main(), ela chamará a leitura das entradas, ou seja, quantidade de linhas, colunas e cores e do próprio tabuleiro em si. Por fim, chamará resolve() que resolverá o jogo.
 - A função resolve() executa um laço em que se busca e joga a melhor jogada. Ela será, em seguida, guardada em uma lista de melhores jogadas que será mostrada ao final da execução do código.
 - A função que busca a melhor jogada descobrirá, através de outra função, as possíveis jogadas para dada configuração do tabuelrio. Em seguida, é calculado se haverá previsão de jogadas e quantas deverão ser previstas para cada jogada (essa parte é crucial para o melhor desempenho do código e envolve uma expressão matemática obtida através de testes empíricos descritos mais para frente nesse README). Roda-se, então, um laço para jogadas possíveis, testando-as e guardando a heurística relativa a cada jogada em uma lista que associa cor a uma heurística. Assim que temos todas nossas possíveis jogadas devidamente testadas, escolhemos a de menor score (detalhes da heurística) e a retornamos como sendo a melhor jogada.

## Heurística 
  - Há duas heurística aqui: número de regiões do tabuleiro dividido pelo número de cores e distância de cada ponto do tabuleiro referente ao cluster principal. Uma região é um cluster de uma só cor, ou seja, em um tabuleiro 6 por 6, por exemplo, podem haver, no máximo, 6x6 regiões, sendo cada ponto de uma cor diferente de modo que elas nunca formam uma região maior que um. 
  - A primeira heurística conta quantas regiões temos em um tabuleiro e divide esse valor pelo número de cores, dando-nos um valor real (float) que estima quantas áreas faltam ser abertas para resolvermos o tabuleiro, a melhor jogada é a que diminui o número total de áreas no tabuleiro.
  - A segunda heurística, e essa sim é nosso "carro chefe", calcula a distância de cada ponto da matriz em relação ao nosso nodo principal, a melhor jogada é a que diminui a maior distância. 

  - Essas heurísticas são somadas, a jogada que tem a menor soma é a melhor. É interessante a heurística ser um valor real, porque não precisamos criar critério de desempate: se duas jogadas diminuem a mesma distância do nodo mais longe, será jogada a que abre mais regiões devido a primeira heurística descrita ser um valor real somado à uma distância natural (inteira positiva). Se ainda assim empatamos, então as duas jogadas são boas.

## Prevendo a melhor jogada
  - Se considerarmos um tabuleiro da seguinte forma:

   1 2 2 1

   3 1 2 1

   2 1 1 1

   1 1 1 1

  Teremos quatro possíveis soluções: 
  
  1-> 2 1 3 2
  
  2-> 2 1 2 3
  
  3-> 3 2 1
  
  4-> 3 1 2

  Perceba que as duas últimas resolvem o tabuleiro mais rapidamente do que as duas primeiras, ou seja, seria ideal se nosso código conseguisse atingí-la através da heurística. Porém, infelizmente, a heurística não resolverá esse tabuleiro de forma ideal. Conlcui-se, portanto, que o único modo de se fazer isso é olhando, para cada jogada possível, qual dela resultará na melhor composição do tabuleiro daqui a algumas jogadas. Para isso, a função preveJogada() foi criada.

  ### preveJogada() 
  - Primeiro, recebemos um tabuleiro em que foi executada alguma jogada possível. Em seguida, vemos quais são as possíveis jogadas para nosso tabuleiro, rodamos um loop para cada qual dessas e guardamos cada jogada associada a uma heurística em uma lista. Depois, escolhemos a melhor jogada dentre as possíveis e a aplicamos no tabuleiro para que possamos rodar todo esse passo mais uma vez (caso seja interessante). 

  - Sabemos quantas jogadas devemos prever no futuro de acordo com a equação _número de previsoes_ = -4.7961 * ln(_número de possíveis cores_) + 14.715. Essa equação foi descoberta através de testes empíricos, ela dá 0 para 20 possíveis cores, quando não prevemos nenhuma jogada e somente usamos a heurística a fim de manter o código dentro de nossa janela de dois minutos. 
   #### O teste empírico
   - O teste empírico foi realizado da seguinda forma: um script foi rodado gerando 5 tabuleiros aleatórios de tamanho 100 100 com k cores, a variável k foi sendo incrementada começando com 4 até 20, ou seja, tabuleiros diferentes até 100 x 100 com 20 cores foram gerados e concluiu-se que, com a previsão de uma única jogada no futuro, seria impossível rodar a IA em menos de 2 minutos. Então, a fim de otimizar nosso tempo, marcamaos o ponto (20,0) em um gráfico de previsões no futuro (eixo das ordenadas) por cores possíveis (eixo das abscissas). Esse teste revelou que, prevendo uma jogada no futuro, o limite seria 19 cores. Com duas previsões no futuro, finalmente, seria 14 cores. Marcou-se 3 pontos ( {(20,0);(19,1);(14,2)} ) e gerou-se uma curva que se aproximava de todos os pontos. Essa curva é descrita pela equação supracitada de _número de previsoes_.

  - Ao final, retornamos uma heurística que será associada à jogada que chamou a função de previsão. Também somamos com a heurística qual profundidade estamos para que a jogada que resolve o tabuleiro antes tenha vantagem quando comparada com as demais. Ou seja, por exemplo, se a olhamos até 4 jogadas no futuro para nosso tabuleiro exemplo descrito acima a previsão constatará que tanto a sequência 3 2 1 quanto 2 1 3 2 resolvem o tabuleiro, porém, a primeira resolveu na terceira previsão, enquanto a segunda resolveu na quarta previsão, isso faz com que as duas jogadas (2 e 3, para a o primeiro movimento) tenham a mesma heurística (afinal, as duas eventualmente resolvem o tabueleiro), porém, se começarmos com 3 terminaremos antes, logo, somamos 3 com a heurística de nossa previsão e contatamos, ao fim, que 3 + h(jogada 3) < 4 + h(jogada2), sendo h(jogada 3) = h(jogada 2) para este caso.

  - SEMPRE tentamos prever para cada jogada quantas jogadas no futuro devemos olhar, para isso aquela equação de número de previsões existe. Mais detalhes nos comentários do código em IAra.cpp.

## Considerações finais
  - Use o comando make para compilar, make clean limpa os .o e make purge limpa todo executável e .o gerado.
  - Você pode, se desejar e eu recomendo, gerar documentação automática usando o comando doxygen. Ele gerará um relatório completo do código em html e latex.

## Para rodar
  - Basta usar o comando make, fazer _./flood_ < **linhas** **colunas** **cores** **matriz_do_tabuleiro**.

#### Autor
 - Feito com carinho, esforço e força de vontade por Richard Fernando Heise Ferreira (rfhf19@inf.ufpr.br) para a disciplina de Inteligência Artifical, ministrada durante o ERE de 2021 devido à pandemia. Espero que goste :) CCQL
