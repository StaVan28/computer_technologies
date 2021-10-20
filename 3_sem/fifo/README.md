# FIFO

Условие:
Создание двух независимых процессов(read, write), который взаимодействуют с помощью FIFO.
wr - процесс считывает файл, rd - выводит дaнные этого файла на экран.

Договоренность:
  -- все процессы знают имя общего FIFO. 


``` 
#--------#        #-----------#        #--------# 
#-  wr  -#  <---  #- DF_FIFO -#  <---  #-  rd  -#
#--------# -      #-----------#     -> #--------#
            -                      -
             -    #-----------#   -
              ->  #- SC_FIFO -#  -
                  #-----------#
```