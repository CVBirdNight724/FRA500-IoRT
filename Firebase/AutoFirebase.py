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
            self.numberFinish += 1
            
            order['time'] = self.time
            order['finish'] = False
            self.numberOrder += 1
            title = self.date + '/' + str(self.numberFinish)
            self.orders[self.numberOrder] = order
            self.host.post(title, order)
            
            
            return True
    
    def getOrders(self, orders):
        for order in orders:
            self.post(order)
        self.host.put('', 'Order', str(self.numberFinish))
        return True
            
    def update(self):
        self.timeUpdate()
        self.host.put('','Date',self.date)
        self.numberFinish = int(self.host.get('Order', None))
        # finish = self.checkFinal()
        # self.host.put('', 'Order', self.numberFinish)

        # start_time = time.time()
        # while True:
        #     if time.time() - start_time > span:
        #         break
    
    def checkFinal(self):
        plan = self.host.get(self.getLatest(), '')
        key = ''
        # print(plan)
        for keys in plan:
            key = keys  
        finish = plan[key]['finish']
        return finish
    
    def getLatest(self):
        ans = '/' + str(self.timeUpdate()) + '/'
        path = ans+str(self.numberFinish)
        return path


# data1 = {
#     'table': '20',
#     'menu': '12',
# }

# data2 = {
#     'table': '5',
#     'menu': '6',
# }


# sys = queueFirebase(fb)
# sys.timeUpdate()
# # sys.checkFinal('/20201210/1')

# sys.update()
# sys.getOrders([data1, data2])
# for i in range(10):
#     state = sys.update()
#     if state is None:
#         break
# print(sys.printData())
