10-clue#3@twg:
	$ for code in 1002 6049 637 15 3000 6151 6083 6075; do echo -ne "${code}\t"; curl -sL "https://www.twgtea.com/product/details?code=T${code}" | egrep "<title>"; done
