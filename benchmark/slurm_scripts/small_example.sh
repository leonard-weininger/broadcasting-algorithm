#!/usr/local_rwth/bin/zsh
#SBATCH --job-name=small_example
#SBATCH --output=output/small_example_%j.txt
#
#SBATCH --cpus-per-task=1
#SBATCH --time=00:15:00
#SBATCH --partition=c23ms
#SBATCH --mem-per-cpu=2000
#
#SBATCH --mail-type=ALL
#SBATCH --mail-user=you@example.com

mkdir -p output

# Hardcoded list of seeds
seeds=(
  141214590102328 443869911735136
  125763611893722 729863777555263
  637625739987609 255771236855876
  934823385813860 206148954000545
  152834004906681 169512904245058
  762603652610086 325137900247599
  242932409204549 691575808195896
  855703693880567 301091827638785
)

seed_index=1

for n in 10 100 1000 10000; do
  for k in {2..4}; do
    current_seed=${seeds[$seed_index]}
    ../benchmark.sh "$n" "$k" "$current_seed"
	echo
	seed_index=$(( seed_index + 1 ))
  done
done

../benchmark.sh 100000 2 549850776303290
