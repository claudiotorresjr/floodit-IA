#/bin/bash

echo "status;teste;jogadas;tempo" >> minhaversao.csv
echo "status;teste;jogadas;tempo" >> professorversao.csv

for i in {1..20}
do
    ../geramapa 100 100 20 > mapa_competicao.tmp

    start_time="$(date -u +%s.%N)"
    ./main < mapa_competicao.tmp > mapa_result.tmp
    end_time="$(date -u +%s.%N)"
    elapsed="$(bc <<<"$end_time-$start_time")"

    cat mapa_competicao.tmp mapa_result.tmp | ../verifica

    jogadas=$(cat mapa_result.tmp | head -n 1)
    if [ "$?" -eq 0 ]
      then
        echo "OK;$i;$jogadas;$elapsed" >> minhaversao.csv
      else
        echo "FAIL;$i;$jogadas;$elapsed"
    fi

    #--------------------------------------------------------------------

    start_time="$(date -u +%s.%N)"
    ../floodit_h4 < mapa_competicao.tmp > mapa_result.tmp
    end_time="$(date -u +%s.%N)"
    elapsed="$(bc <<<"$end_time-$start_time")"

    jogadas=$(cat mapa_result.tmp | head -n 1)
    echo "OK;$i;$jogadas;$elapsed" >> professorversao.csv
done