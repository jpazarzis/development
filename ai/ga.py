#!/usr/bin/python   
import random
from race_info.utilities import meanstdv



class Chromosome:
    def __init__(self, length=None, mutator = None):
        ''' 
        length: 
                the size of the chromosome
     
        mutator: a function returning a random weight
                 used upon initialization and mutation
            
        When length is None it creates an empty object that
        will be populatated by hand.  This will happend when 
        a crossover will occur
        '''
        if not length :
            self.genes = None
        else:
            self.genes = [mutator() for i in range(length) ]
        
        self.fitness = None

    def calculate_fitness(self, fitness_calculator):
        '''
        fitness_calculator : 
            calculates the fitness based in the current
            weights as appear in genes
        '''
        self.fitness = fitness_calculator(self.genes)

    def __str__(self):
        return ' '.join( [ '{0:2.3f}'.format(g) for g in self.genes] ) + ' Fitness: {0}'.format(self.fitness)

def breed_chromosomes(c1,c2):
    '''crosses over c1, c2 returns a tuple of two new chromosomes'''
    g1, g2 = c1.genes, c2.genes
    assert(len(g1) == len(g2))
    new_c1, new_c2 = Chromosome(), Chromosome()
    cross_over_point = random.randint(1,len(g1)-2)
    new_c1.genes = g1[0:cross_over_point] + g2[cross_over_point:]
    new_c2.genes = g2[0:cross_over_point] + g1[cross_over_point:]
    return new_c1, new_c2

def mutator_decorator(min_value, max_value):
    ''' returns a function to generate random value between 
    min_value, max_value '''
    def foo():
        return random.uniform(min_value,max_value)
    return foo

def chromosome_factory_decorator(length, mutator):
    ''' returns a function to create a new random chromosome'''
    def foo():
        return Chromosome(length, mutator)
    return foo


def fitness_calculator_sample(weights):
    ''' used only for testing'''
    return sum(weights)

class GeneticAlgorithm:
    def __init__(self, fitness_calculator, chromosome_length ,number_of_chromosomes = 50, min_value = 1, max_value = 100 , elitism_factor = 0.05, mutation_rate = 0.18, stop_level = 200):
        mutator = mutator_decorator(min_value,max_value)
        chromosome_factory = chromosome_factory_decorator(chromosome_length,mutator)
        self.chromosomes = [ chromosome_factory() for i in range(number_of_chromosomes)]
        self.fitness_calculator = fitness_calculator
        self.elitism_factor = elitism_factor
        self.mutator = mutator
        self.mutation_rate = mutation_rate
        self.stop_level = stop_level

    def winner(self):
        ''' since each generation has some elite chromosomes in the top
            the first one is guranteed to be the fittest '''
        return self.chromosomes[0].genes

    def winning_chromosome(self):
        return self.chromosomes[0]

    def mutate(self,chromosome):
        for i in range(len(chromosome.genes)):
            if random.random() <= self.mutation_rate:
                chromosome.genes[i] = self.mutator()


    def run(self, max_reps = 200):
        counter, previous_fitness = 0, None
        for i in range(200):
            self.make_next_generation()
            f = self.current_fitness()
            if not previous_fitness:
                previous_fitness, counter = f,1 
            elif previous_fitness == f:
                counter += 1
            else:
                previous_fitness, counter = f, 0

            if counter >= self.stop_level:
                break

    def current_fitness(self):
        return self.winning_chromosome().fitness

    def make_next_generation(self):
        self.calculate_fitness()
        mean, stdev = meanstdv([c.fitness for c in self.chromosomes])
        indexes_to_chromosomes_to_breed = []
        for index, c in enumerate(self.chromosomes):
            if c.fitness >= mean + 2 * stdev:
                indexes_to_chromosomes_to_breed.extend([index]*6)
            elif c.fitness >= mean + stdev:
                indexes_to_chromosomes_to_breed.extend([index]*3)
            elif c.fitness >= mean :
                indexes_to_chromosomes_to_breed.extend([index] *2)

        self.chromosomes.sort(key= lambda c: c.fitness, reverse = True)
        new_generation = self.chromosomes[0: 1 + int(len(self.chromosomes) * self.elitism_factor)]

        while len(new_generation) < len(self.chromosomes):
            i1 = random.choice(indexes_to_chromosomes_to_breed)
            i2 = random.choice(indexes_to_chromosomes_to_breed)
            if i1 == i2:
                continue
            c1, c2 = breed_chromosomes(self.chromosomes[i1], self.chromosomes[i2])
            self.mutate(c1)
            self.mutate(c2)
            new_generation.extend([c1,c2])

        self.chromosomes = new_generation[0:len(self.chromosomes)]

        print self

    def calculate_fitness(self):
        for c in self.chromosomes:
            c.calculate_fitness(self.fitness_calculator)

    def __str__(self):
        s = '\nnumber of chromosomes: {0:3} \nElitism Factor: {1:2.2f} \nMutation Rate: {2:2.2f}\n'.format(len(self.chromosomes), self.elitism_factor,  self.mutation_rate)
        return s+'\n winner chromosome: \n {0}\n'.format(str(self.chromosomes[0]))
        #return  '\n'.join( [ str(self.chromosomes[0]) ] )

if __name__ == '__main__':
    mutator = mutator_decorator(0,1)
    chromosome_factory = chromosome_factory_decorator(7,mutator)

    p = GeneticAlgorithm(10,chromosome_factory,fitness_calculator_sample,mutator)

    for i in range(10):
        p.make_next_generation()

