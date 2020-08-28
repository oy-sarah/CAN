# Copyright (c) XX).
# All rights reserved.
#
"""
Created on 2020-08-11

@author: XX

usage : 
    python CAN.py dev
"""
import dgl
import numpy as np
import sys
from configparser import ConfigParser
import logging.config
import math

import networkx as nx
import torch.nn as nn
import torch.nn.functional as F
import torch
import itertools
# initialize logger
logging.config.fileConfig("logging.cfg")
logger = logging.getLogger("root")

class RuntimeContext(object):
    """ runtime enviroment
    """
    
    def __init__(self):
        """ initialization
        """
        # configuration initialization
        config_parser = ConfigParser()
        config_file = self.get_config_file_name()
        config_parser.read(config_file, encoding="UTF-8")
        sections = config_parser.sections()
        
        coverage_information_matrix_section = sections[0]
        self.covMatrix = config_parser.get(coverage_information_matrix_section, "covMatrix")
        
        test_cases_results_section = sections[1]
        self.error = config_parser.get(test_cases_results_section, "error")
        
        DL_result_section = sections[2]
        self.DL_result = config_parser.get(DL_result_section, "DL_result")
        
        parameters = sections[3]
        self.learning_rate = config_parser.get(parameters, "learning_rate")
        self.drop = config_parser.get(parameters, "drop")
        self.epochs_drop = config_parser.get(parameters, "epochs_drop")
        self.batch_size = config_parser.get(parameters, "batch_size")
        self.in_units = config_parser.get(parameters, "in_units")
        self.test_num = config_parser.get(parameters, "test_num")
        
    def get_config_file_name(self):
        """ get the configuration file name according to the command line parameters
        """
        argv = sys.argv
        config_type = "dev" # default configuration type
        if None != argv and len(argv) > 1 :
            config_type = argv[1]
        config_file = config_type + ".cfg"
        logger.info("get_config_file_name() return : " + config_file)
        return config_file



def FL_subject_graph():
    runtime_context = RuntimeContext()
    g = dgl.DGLGraph()
    # add 16 nodes into the graph; nodes are labeled from 0~33
    g.add_nodes(int(runtime_context.in_units))
    # all 25 edges as a list of tuples
    edge_list = [(0, 49), (0, 56), (1, 3), (1, 5), (1, 24),(1, 50),
(2, 3), (2, 5), (2, 6), (2, 11), (2, 15),(2, 17), (2, 22),(2, 23),
(2, 24), (2, 25), (2, 26), (2, 27), (2, 29),(2, 31), (2, 33),(2, 36),
(2, 37), (2, 39), (2, 41), (2, 42), (2, 43),(2, 44), (2, 45),(2, 49),
(2, 50), (2, 52), (2, 53), (2, 54), (2, 55),(2, 56), (5, 6),(5, 7),
(5, 8), (5, 9), (5, 10), (6, 7), (7, 8),(7, 9), (9, 10),(11, 12),
(12, 13), (12, 15), (13, 14), (13, 15), (13, 16),(17, 18), (18, 19),(19, 20),
(19, 21), (20, 21), (23, 24), (27, 28), (27, 29),(27, 32), (32, 33),(34, 35),
(34, 36), (37, 38), (38, 39), (38, 40), (38, 41),(38, 42), (38, 43),(38, 44),
(38, 45), (38, 46), (38, 47), (38, 48), (38, 49),(38, 50), (38, 51),(39, 40),
(40, 41), (40, 42), (40, 43), (40, 44), (40, 45),(40, 46), (42, 45),(42, 47),
(39, 48), (47, 48),(49, 50), (49, 51), 
(22, 21), (21, 14), (14, 10), (10, 4)]
    # add edges two lists of nodes: src and dst
    src, dst = tuple(zip(*edge_list))
    g.add_edges(src, dst)
    # edges are directional in DGL; make them bi-directional
    #g.add_edges(dst, src)

    return g

def gnn_message(edges):
    # The argument is a batch of edges.
    # This computes a (batch of) message called 'msg' using the source node's feature 'h'.
    return {'msg' : edges.src['h']}

def gnn_reduce(nodes):
    # The argument is a batch of nodes.
    # This computes the new 'h' features by summing received 'msg' in each node's mailbox.
    return {'h' : torch.sum(nodes.mailbox['msg'], dim=1)}

# Define the GNNLayer module
class GNNLayer(nn.Module):
    def __init__(self, in_feats, out_feats):
        super(GNNLayer, self).__init__()
        self.linear = nn.Linear(in_feats, out_feats)

    def forward(self, g, inputs):
        # g is the graph and the inputs is the input node features
        # first set the node features
        g.ndata['h'] = inputs
        # trigger message passing on all edges
        g.update_all(gnn_message,gnn_reduce)
        # get the result node features
        h = g.ndata.pop('h')
        # perform linear transformation
        return self.linear(h)
# Define a 2-layer GNN model
class GNN(nn.Module):
    def __init__(self, in_feats, hidden_size, num_classes):
        super(GNN, self).__init__()
        runtime_context = RuntimeContext()
        INUNITS = int(runtime_context.in_units)
        self.gnn1 = GNNLayer(in_feats, hidden_size)
        self.gnn2 = GNNLayer(hidden_size, num_classes)
        self.linear_2 = nn.Linear(INUNITS, 1)

    def forward(self, g, inputs):
        h = self.gnn1(g, inputs)
        h = torch.relu(h)
        h = self.gnn2(g, h)
        h = torch.transpose(h,1,0)
        h = self.linear_2(h)
        h = torch.sigmoid(h)
        return h




def main():
    '''
    load data of test cases and test results:
    '''
    runtime_context = RuntimeContext()
    INUNITS = int(runtime_context.in_units)
    TESTNUM = int(runtime_context.test_num)
    
    f1 = open(runtime_context.covMatrix,'r')
    f2 = open(runtime_context.error,'r')
    f3 = open(runtime_context.DL_result,'w')
    
    first_ele = True
    for data in f1.readlines():
        data = data.strip('\n')
        nums = data.split()
        if first_ele:
            nums = [float(x) for x in nums]
            matrix_x = np.array(nums)
            first_ele = False
        else:
            nums = [float(x) for x in nums]
            matrix_x = np.c_[matrix_x,nums]
    f1.close()
    
    first_ele = True
    for data in f2.readlines():
        data = data.strip('\n')
        nums = data.split()
        if first_ele:
            nums = [float(x) for x in nums]
            matrix_y = np.array(nums)
            first_ele = False
        else:
            nums = [float(x) for x in nums]
            matrix_y = np.c_[matrix_y,nums]
    f2.close()
    
    
    matrix_x = matrix_x.transpose()
    matrix_y = matrix_y.transpose()
    
    '''
    load graph:
    '''
    G = FL_subject_graph()
    logger.info('We have %d nodes.' % G.number_of_nodes())
    logger.info('We have %d edges.' % G.number_of_edges())
    
    
    '''
    embeddings for all nodes:
    '''
    G.ndata['embed'] = torch.eye(INUNITS)
    
    '''
    draw the graph
    '''
    nx_G = G.to_networkx().to_undirected()
    pos = nx.kamada_kawai_layout(nx_G)
    nx.draw(G.to_networkx(), with_labels=True)
    '''
    initialize the network and parameters
    '''
    net = GNN(INUNITS, 5, 1)
    
    inputs = torch.FloatTensor(matrix_x)
    labels = torch.FloatTensor(matrix_y)
    
    loss_func = nn.MSELoss() 
    zero_tensor = torch.zeros(INUNITS, requires_grad=True)
    input_embed = torch.ones([INUNITS, INUNITS],requires_grad=True)
    
    learning_rate = float(runtime_context.learning_rate)
    drop = float(runtime_context.drop)
    epochs_drop = float(runtime_context.epochs_drop)
    '''
    start training
    '''
    for epoch in range(20):
        epoch_loss = 0
        optimizer = torch.optim.Adam(itertools.chain(net.parameters(), [input_embed]), lr=learning_rate)
        for index in range(len(labels)):
            for i in range(len(inputs[index])):
                if inputs[index][i] == 0:
                    input_embed[i] = zero_tensor.clone()
                else:
                    input_embed[i] = G.ndata['embed'][i].clone()
            input_embed = torch.tensor(input_embed, requires_grad=True)
            optimizer1 = torch.optim.Adam(itertools.chain(net.parameters(), [input_embed]), lr=learning_rate)
            for iteration in range(3):
                prediction = net(G, input_embed)
                loss = loss_func(prediction[0], labels[index])
                optimizer.zero_grad()
                loss.backward()
                optimizer.step()
            epoch_loss += loss.item()
            
            for i in range(len(inputs[index])):
                if inputs[index][i] == 1:
                    G.ndata['embed'][i] = input_embed[i].clone()
                      
        epoch_loss /= (index + 1)
        if epoch%epochs_drop == 0:
            learning_rate = learning_rate * math.pow(drop,math.floor((epoch)/epochs_drop))
    
        logger.info('Epoch %d | Loss: %.4f' % (epoch, epoch_loss))
    
    
    """
    get fault localization result
    """   
    test_input = torch.eye(INUNITS)
    for index in range(len(test_input)):
        input_embed = nn.Embedding(INUNITS, INUNITS).weight
        for i in range(len(test_input[index])):
                if test_input[index][i] == 0:
                    input_embed[i] = 0
                else:
                    input_embed[i] = G.ndata['embed'][i]
        prediction = net(G, input_embed)
        loss = loss_func(prediction[0], torch.tensor([1.]))
        f3.write(str(prediction.item()))
        f3.write('\n')
        
if __name__ == "__main__":
    main()
