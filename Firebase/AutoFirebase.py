from firebase import firebase
import time

fb = firebase.FirebaseApplication('https://dinning-robot.firebaseio.com/', None)

class queueFirebase:
    def __init__(self, host):
        self.host = host
        self.date = ''
        self.time = ''
        self.numberOrder = 0
        self.numberFinish = 0
        self.orders = {}
            
    def timeUpdate(self):
        localtime = time.localtime(time.time())
        self.date = str(localtime[0]) + str(localtime[1]) + str(localtime[2])
        self.time = str(localtime[3]) + str(localtime[4]) + str(localtime[5])
        return self.date

    def post(self, order=None):
        if order != '' and type(order) != type(None):
            self.timeUpdate()
            order['time'] = self.time
            order['finish'] = False
            self.numberOrder += 1
            title = self.date + '/' + str(self.numberOrder)
            self.orders[self.numberOrder] = order
            self.host.post(title, order)
            return True

    def delete(self):
        title = self.date + '/' + str(self.numberFinish + 1)
        data = self.host.get(title, '')
        if data is None:
            return None
        for key in data:
            if data[key]['finish'] == True:
                self.host.delete(title, '')
                self.numberFinish += 1
                return True
            else:
                return False
    
    def getOrders(self, orders):
        for order in orders:
            self.post(order)
        return True
            
    def update(self, span=10):
        self.timeUpdate()
        self.delete()
        start_time = time.time()
        while True:
            if time.time() - start_time > span:
                break

    def printData(self, order=None):
        if order is None:
            return self.orders
        else:
            orders = {}
            for key in order:
                orders[key] = self.orders[key]
            return orders


data1 = {
    'table': '5',
    'menu': '11',
}

data2 = {
    'table': '3',
    'menu': '4',
}

sys = queueFirebase(fb)

sys.getOrders([data1, data2])
for i in range(10):
    state = sys.update()
    if state is None:
        break
print(sys.printData())