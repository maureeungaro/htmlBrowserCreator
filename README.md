# pageCreator


# Usage

First a file (by default called imageslist.txt) with the list of images to be 
displayed should be created. Typically:

```
rm -rf imageslist.txt ; ls img > imageslist.txt
```

The row column can be selected using the -addRowTableVariable option. 

**pageCreator** will  look for a string in the image name after the row variable, 
until it finds an underscore "_". 

For example, if the row variable is "cut", we have this images: 


```
img/cut-field-strength1_sector-1.png
img/cut-field-strength2_sector-1.png
```


then there will be two rows: "field-strength1" and "field-strength2".
