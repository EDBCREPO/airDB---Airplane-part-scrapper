#---------------------------------------------------------------------#

FLAG="-lssl -lcrypto -lz -I./include"
LIST=( airdb )

#---------------------------------------------------------------------#

if [ ! -d "build" ]; then
    mkdir "build"
fi

#---------------------------------------------------------------------#

echo -e "compilling airdb"
for ITEM in "${LIST[@]}" ; do

    if [ -f "./build/$ITEM" ]; then
        echo "-> compiled   $ITEM"; continue
    fi; echo "-> compilling $ITEM"

    g++ -o ./build/$ITEM ./service/$ITEM.cpp $FLAG

    if [ ! $? -eq 0 ]; then
        echo "exit error"; exit;
    fi

done

#---------------------------------------------------------------------#

echo -e "running airdb" ; ./build/airdb ?value=522151-10
