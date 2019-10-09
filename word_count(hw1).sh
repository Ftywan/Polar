COUNT=$(grep $1 /usr/share/dict/words | grep -c $2)
echo "There are $COUNT words that have '$1' and '$2'"
