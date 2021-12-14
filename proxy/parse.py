def parse(data, port, origin):
    if origin == 'server':
        print "[" + origin + ":" + str(port) + "]" + data
    if port == 5555: #c'est impossible en l'occurence mais c'est pour l'exemple
        return
    else:
        print "[" + origin + ":" + str(port) + "]" + data