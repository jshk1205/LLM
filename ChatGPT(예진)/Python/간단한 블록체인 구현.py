import hashlib
import json
from time import time
from uuid import uuid4
import copy
import base64
from urllib.parse import urlparse
import requests
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.asymmetric import padding
from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import serialization

class Blockchain:
    def __init__(self):
        self.chain = []
        self.current_transactions = []
        self.difficulty = 4  # 난이도 조절을 위한 변수
        self.mining_reward = 1  # 채굴 보상
        self.nodes = set()

        # 제네시스 블록 생성
        self.new_block(previous_hash='1', proof=100)

    def register_node(self, address):
        """
        네트워크에 새 노드 추가
        :param address: 노드의 주소 (예: 'http://192.168.0.5:5000')
        """
        parsed_url = urlparse(address)
        self.nodes.add(parsed_url.netloc)

    def valid_chain(self, chain):
        """
        블록체인의 유효성을 검증
        :param chain: 블록체인
        :return: 유효하면 True, 그렇지 않으면 False
        """
        last_block = chain[0]
        current_index = 1

        while current_index < len(chain):
            block = chain[current_index]
            if block['previous_hash'] != self.hash(last_block):
                return False

            # Proof of Work 검증
            if not self.valid_proof(last_block['proof'], block['proof']):
                return False

            last_block = block
            current_index += 1

        return True

    def resolve_conflicts(self):
        """
        이 알고리즘은 가장 긴 유효한 블록체인으로 대체됩니다.
        :return: 체인이 대체되었으면 True, 그렇지 않으면 False
        """
        neighbors = self.nodes
        new_chain = None

        # 네트워크에서 가장 긴 체인 찾기
        max_length = len(self.chain)

        # 네트워크의 다른 노드 체인 검색
        for node in neighbors:
            response = requests.get(f'http://{node}/chain')

            if response.status_code == 200:
                length = response.json()['length']
                chain = response.json()['chain']

                # 길이가 더 긴 블록체인이 발견되면 검증 후 대체
                if length > max_length and self.valid_chain(chain):
                    max_length = length
                    new_chain = chain

        # 새로운 체인이 발견되었고 유효하면 체인을 대체
        if new_chain:
            self.chain = new_chain
            return True

        return False

    def new_block(self, proof, previous_hash=None):
        """
        새 블록 생성
        :param proof: Proof of Work 알고리즘에 의해 생성된 증명
        :param previous_hash: 이전 블록의 해시
        :return: 새 블록
        """
        block = {
            'index': len(self.chain) + 1,
            'timestamp': time(),
            'transactions': self.current_transactions,
            'proof': proof,
            'previous_hash': previous_hash or self.hash(self.chain[-1]),
        }

        # 현재 트랜잭션 초기화
        self.current_transactions = []

        # 체인에 블록 추가
        self.chain.append(block)
        return block

    def new_transaction(self, sender, recipient, amount, signature):
        """
        새 트랜잭션 생성
        :param sender: 보내는 사람 주소
        :param recipient: 받는 사람 주소
        :param amount: 전송량
        :param signature: 서명
        :return: 이 트랜잭션을 포함하는 블록의 인덱스
        """
        self.current_transactions.append({
            'sender': sender,
            'recipient': recipient,
            'amount': amount,
            'signature': signature
        })

        return self.last_block['index'] + 1

    def proof_of_work(self, last_proof):
        """
        간단한 Proof of Work 알고리즘:
         - hash(pp')의 마지막 4개가 0이 되도록 하는 p'를 찾습니다.
         - p는 이전의 proof, p'는 새로운 proof입니다.
        :param last_proof: 이전의 proof
        :return: 새로운 proof
        """
        proof = 0
        while self.valid_proof(last_proof, proof) is False:
            proof += 1

        return proof

    @staticmethod
    def valid_proof(last_proof, proof):
        """
        Proof of Work의 유효성을 검증합니다.
        :param last_proof: 이전의 proof
        :param proof: 현재의 proof
        :return: 유효하면 True, 그렇지 않으면 False
        """
        guess = f'{last_proof}{proof}'.encode()
        guess_hash = hashlib.sha256(guess).hexdigest()
        return guess_hash[:4] == "0" * blockchain.difficulty

    @staticmethod
    def hash(block):
        """
        블록의 SHA-256 해시 생성
        :param block: 블록
        :return: 해시
        """
        block_string = json.dumps(block, sort_keys=True).encode()
        return hashlib.sha256(block_string).hexdigest()

    @property
    def last_block(self):
        return self.chain[-1]

    def mine_block(self, miner_address):
        """
        블록 채굴 메서드
        - 새로운 블록을 채굴하여 체인에 추가하고 보상을 지급하는 역할을 수행
        :param miner_address: 보상을 받을 채굴자의 주소
        :return: 채굴된 블록의 정보를 반환
        """
        last_block = self.last_block
        last_proof = last_block['proof']
        proof = self.proof_of_work(last_proof)

        # 보상을 포함한 새로운 트랜잭션 생성
        self.new_transaction(
            sender="0",  # 새로운 코인의 발행자를 나타내는 임시 주소
            recipient=miner_address,
            amount=self.mining_reward,
            signature=""  # 보상 트랜잭션은 서명이 필요하지 않음
        )

        # 채굴에 성공하여 새로운 블록 추가
        previous_hash = self.hash(last_block)
        block = self.new_block(proof, previous_hash)

        return block

# 트랜잭션 서명에 사용될 RSA 키 생성 및 관련 메서드
def generate_rsa_key_pair():
    private_key = rsa.generate_private_key(
        public_exponent=65537,
        key_size=2048,
        backend=default_backend()
    )
    public_key = private_key.public_key()
    return private_key, public_key

def serialize_private_key(private_key):
    return private_key.private_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PrivateFormat.PKCS8,
        encryption_algorithm=serialization.NoEncryption()
    )

def serialize_public_key(public_key):
    return public_key.public_bytes(
        encoding=serialization.Encoding.PEM,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )

# RSA 키 생성
miner_private_key, miner_public_key = generate_rsa_key_pair()

# 블록체인 인스턴스 생성
blockchain = Blockchain()

# 채굴자 주소
miner_address = serialize_public_key(miner_public_key)

# 새로운 트랜잭션 생성 및 서명
transaction = {
    'sender': serialize_public_key(miner_public_key).decode(),
    'recipient': 'recipient_address',
    'amount': 1.5
}
transaction_string = json.dumps(transaction, sort_keys=True)
transaction_signature = miner_private_key.sign(
    transaction_string.encode(),
    padding.PSS(
        mgf=padding.MGF1(hashes.SHA256()),
        salt_length=padding.PSS.MAX_LENGTH
    ),
    hashes.SHA256()
)
transaction_signature_hex = transaction_signature.hex()

# 트랜잭션 서명과 함께 블록에 추가
blockchain.new_transaction(transaction['sender'], transaction['recipient'], transaction['amount'], transaction_signature_hex)

# 블록 채굴
mined_block = blockchain.mine_block(miner_address)

print("블록체인: ", blockchain.chain)
