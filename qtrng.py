from qiskit import QuantumRegister, ClassicalRegister
from qiskit import QuantumCircuit, Aer, execute


q1 = QuantumRegister(1);
c1 = ClassicalRegister(1);

q2 = QuantumRegister(2);
c2 = ClassicalRegister(2);

qc = QuantumCircuit(q1, c1);
backend = Aer.get_backend("qasm_simulator");

qc.h(q1);
qc.measure(q1, c1);

for i in range(0, 100):
    qjob = execute(qc, backend, shots=1);
    counts = qjob.result().get_counts();
    for key in counts.keys():
        print(key);

