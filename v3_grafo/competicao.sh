#/bin/bash

# echo "status;teste;jogadas;tempo" > minhaversao.csv
# echo "status;teste;jogadas;tempo" > professorversao.csv
for i in {1..100}
do
  for j in {1..100}
  do
    total_c=$(( i*j ))
    for c in $(seq 1 $total_c)
    do
        echo "$i x $j com $c cores..."
        ../geramapa $i $j $c > mapa_competicao.tmp

        start_time="$(date -u +%s.%N)"
        ./floodit < mapa_competicao.tmp > mapa_result.tmp
        end_time="$(date -u +%s.%N)"
        my_elapsed="$(bc <<<"$end_time-$start_time")"

        cat mapa_competicao.tmp mapa_result.tmp | ../verifica

        if [ "$?" -eq 0 ]
          then
            jogadas_meu=$(cat mapa_result.tmp | head -n 1)
            #echo "OK $i $jogadas $elapsed" #>> minhaversao.csv
          else
            echo "FAIL $i $jogadas $elapsed"
        fi

        #--------------------------------------------------------------------

        # start_time="$(date -u +%s.%N)"
        # ../floodit_h4 < mapa_competicao.tmp > mapa_result.tmp
        # end_time="$(date -u +%s.%N)"
        # prof_elapsed="$(bc <<<"$end_time-$start_time")"

        # jogadas_prof=$(cat mapa_result.tmp | head -n 1)

        # if [[ "$prof_elapsed" > "$my_elapsed" ]]
        #   then
        #     echo "GANHEI no tempo com $my_elapsed vs $prof_elapsed"
        #   elif [[ "$prof_elapsed" == "$my_elapsed" ]]
        #   then
        #     echo "EMPATEI no tempo com $my_elapsed vs $prof_elapsed"
        #   else
        #     echo "PERDI no tempo com $my_elapsed vs $prof_elapsed"
        # fi

        # if [[ "$jogadas_prof" > "$jogadas_meu" ]]
        #   then
        #     echo "GANHEI nas jogadas com $jogadas_meu vs $jogadas_prof"
        #   elif [[ "$jogadas_prof" == "$jogadas_meu" ]]
        #   then
        #     echo "EMPATEI nas jogadas com $jogadas_meu vs $jogadas_prof"
        #   else
        #     echo "PERDI nas jogadas com $jogadas_meu vs $jogadas_prof"
        # fi
        # #echo "OK $i $jogadas $elapsed" #>> professorversao.csv

        if [ "$c" -eq 20 ]
        then
          break
        fi

    done
  done
done

# gnuplot <<- EOF
#     set xlabel "Numero do teste"
#     set ylabel "Cores para solucionar"
#     set title "Cores que solucionam o tabuleiro 100x100 20 cores"   
#     set style data point
#     set style function line
#     set xtics 1

#     set style line 1 lc rgb "blue" lw 2
#     set style line 2 lc rgb "green" lw 2

#     set term png
#     set output "cores.png"
#     plot "minhaversao.csv" using 2:3 ls 1 title 'minhaversao' with lines, \
#     "professorversao.csv" using 2:3 ls 2 title 'professorversao' with lines
# EOF

# gnuplot <<- EOF
#     set xlabel "Numero do teste"
#     set ylabel "Tempo para solucionar"
#     set title "Tempo de execucao para solucionar o tabuleiro 100x100 20 cores"   
#     set style data point
#     set style function line
#     set xtics 1

#     set style line 1 lc rgb "blue" lw 2
#     set style line 2 lc rgb "green" lw 2

#     set term png
#     set output "tempo.png"
#     plot "minhaversao.csv" using 2:4 ls 1 title 'minhaversao' with lines, \
#     "professorversao.csv" using 2:4 ls 2 title 'professorversao' with lines
# EOF