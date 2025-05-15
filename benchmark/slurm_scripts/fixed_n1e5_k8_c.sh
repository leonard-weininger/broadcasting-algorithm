#!/usr/local_rwth/bin/zsh
#SBATCH --job-name=fixed_n1e5_k8_c
#SBATCH --output=output/fixed_n1e5_k8_c_%j.txt
#
#SBATCH --cpus-per-task=1
#SBATCH --time=50:00:00
#SBATCH --partition=c23ms
#SBATCH --mem-per-cpu=2000
#
#SBATCH --mail-type=ALL
#SBATCH --mail-user=you@example.com

mkdir -p output

../benchmark.sh 100000 8 665572603953321
