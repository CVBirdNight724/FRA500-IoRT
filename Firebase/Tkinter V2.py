import tkinter as tk
from AutoFirebase import queueFirebase
from firebase import firebase
from threading import Thread
import time

fb = firebase.FirebaseApplication('https://dinning-robot.firebaseio.com/', None)
FB = queueFirebase(fb)

FB.update()
data1 = {
    'patient': 'John Smith',
    'age': '20',
    'menu': 'Egg toast',
    'table': '1'
}
data2 = {
    'patient': 'John Smith',
    'age': '20',
    'menu': 'Egg toast',
    'table': '2'
}
data3 = {
    'patient': 'John Smith',
    'age': '20',
    'menu': 'Egg toast',
    'table': '3'
}
data4 = {
    'patient': 'John Smith',
    'age': '20',
    'menu': 'Egg toast',
    'table': '4'
}

# def add_log(texts):
#     text.insert(tk.END, texts+"\n")
#     entry_add.delete(0,tk.END)
#     return None

# def delete_log(x):
#     text.delete('1.0', '2.0')
#     return None

def ChangeColor(Color):
    if Color == 'R':
        C.itemconfig(oval_red, fill='red')
        C.itemconfig(oval_green, fill='white')
    elif Color == 'G':
        
        C.itemconfig(oval_red, fill='white')
        C.itemconfig(oval_green, fill='green')
    
def thread():
    while(True):
        try:
            FB.update()
            if FB.checkFinal() is True:
                ChangeColor('G')
            elif FB.checkFinal() is False:
                ChangeColor('R')
            else: 
                pass
        except:
            pass


thr1 = Thread(target=thread, args=[])
thr1.start()

def sendOrder(data):
    ChangeColor('R')
    FB.getOrders(data)
    


window = tk.Tk()
frame = tk.Frame(window)
frame.pack()
# window.columnconfigure([0,5,10], weight=1, minsize=20)
# window.columnconfigure([1,3,5,6,8,9], weight=2, minsize=50)
# window.columnconfigure([2,4,7], weight=4, minsize=150)
# window.rowconfigure([0,2,4,6,7,8,9], weight=1, minsize=25)
# window.rowconfigure([1,3,5], weight=2, minsize=50)


#-----------------------------fill----------------------------
# for i in range(10):
#     for j in range(11):
#         label = tk.Label(text="Row"+str(i)+"\ncolumn"+str(j))
#         # label = tk.Label(text='')
#         label.grid(row=i,column=j, padx=5, pady=5,sticky='nsew')

# log = tk.Label(text='Log')
# log.grid(row=0,column=6,columnspan=4,padx=5, pady=5, sticky='nsew')

button1 = tk.Button(frame, text="1", command=lambda: sendOrder([data1]), width=2)
# button1.pack()
button1.grid(row=2, column=1, padx=5, pady=5, sticky='nsew')

# entry1 = tk.Entry()
# entry1.grid(row=2, column=2, padx=5, pady=5, sticky='nsew')

button2 = tk.Button(
    frame, text="2", command=lambda: sendOrder([data2]), width=2)
# button2.pack()
button2.grid(row=2, column=3, padx=5, pady=5, sticky='nsew')

# entry2 = tk.Entry()
# entry2.grid(row=2, column=4, padx=5, pady=5, sticky='nsew')

button3 = tk.Button(
    frame, text="3", command=lambda: sendOrder([data3]), width=2)
# button3.pack()
button3.grid(row=4, column=1, padx=5, pady=5, sticky='nsew')

# entry3 = tk.Entry()
# entry3.grid(row=4, column=2, padx=5, pady=5, sticky='nsew')

button4 = tk.Button(
    frame, text="4", command=lambda: sendOrder([data4]), width=2)
# button4.pack()    
button4.grid(row=4, column=3, padx=5, pady=5, sticky='nsew')

button5 = tk.Button(frame, text="Change_Color", command=lambda: ChangeColor('R'), width=10)
# button5.pack()
button5.grid(row=5, column=1, padx=5, pady=5, sticky='nsew')

button6 = tk.Button(frame, text="Change_Color",command=lambda: ChangeColor('G'), width=10)
# button6.pack()
button6.grid(row=5, column=3, padx=5, pady=5, sticky='nsew')

button7 = tk.Button(frame, text="print", command=lambda: FB.checkFinal(), width=10)
# button7.pack()
button7.grid(row=5, column=4, padx=5, pady=5, sticky='nsew')

# entry4 = tk.Entry()
# entry4.grid(row=4, column=4, padx=5, pady=5, sticky='nsew')

# text = tk.Text()
# text.grid(row=1,column=6,rowspan=5, columnspan=4, sticky='nsew')

C = tk.Canvas(window, height=120, width=230, bg='white')
C.pack()

oval_red = C.create_oval(10, 10, 110, 110, fill='red')
oval_green = C.create_oval(120, 10, 220, 110, fill='green')

# ---------------------test log---------------------------
# entry_add = tk.Entry()
# entry_del = tk.Entry()
# entry_get = tk.Entry()
# buttonadd = tk.Button(text="add",command=lambda:add_log(entry_add.get()),width=3)
# buttondel = tk.Button(text="del",command=lambda:delete_log(entry_del.get()),width=3)
# buttonget = tk.Button(text="get",command=lambda:print(text.get("1.0","2.0")),width=3)
# buttonadd.grid(row=6,column=7, padx=5, pady=5, sticky='ensw')
# entry_add.grid(row=6,column=8, padx=5, pady=5, sticky='nsew')
# buttondel.grid(row=7,column=7, padx=5, pady=5, sticky='nsew')
# entry_del.grid(row=7,column=8, padx=5, pady=5, sticky='nsew')
# buttonget.grid(row=8,column=7, padx=5, pady=5, sticky='nsew')
# entry_get.grid(row=8,column=8, padx=5, pady=5, sticky='nsew')
window.mainloop()



