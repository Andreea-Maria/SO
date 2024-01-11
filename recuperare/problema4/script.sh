
if [ "$#" -ne 2 ]; then
  echo "Usage: $0 <source_directory> <test_directory>"
  exit 1
fi

source_directory="$1"
test_directory="$2"

# Parcurge subdirectoarele din directorul sursă
for subdir in "$source_directory"/*/; do
  # Verifică dacă există fișiere C în subdirector
  if ls "$subdir"/*.c &>/dev/null; then
    echo "Compilarea programului din directorul: $subdir"
    
    # Compilează fișierele C din subdirector
    gcc -Wall -o "$subdir/program" "$subdir"*.c
    
    # Verifică codul de eroare al compilării
    if [ $? -eq 0 ]; then
      echo "Compilare cu succes!"
      
      # Parcurge fișierele de test din directorul de teste
      for test_file in "$test_directory"/*.txt; do
        echo "Rularea programului pentru testul: $test_file"
        "$subdir/program" "$test_file"
        echo "------------------------------------"
      done
    else
      echo "Eroare la compilare!"
    fi
  fi
done
