import numpy as np

data = "한국과 중국 정부는 한중관계 개선을 위한 양국 협의결과 발표를 통해 한중관계 2.0 시대의 포문을 열었다. 한중 양국은 서로 다른 외교안보 의식을 인정하는 한편, 상호필요에 의해 전략적 관계를 유지해나간다는 데 합의했다. 한중관계를 경제성장을 목표로 구축됐던 한중 상호협력관계에서 안보 변수를 반영한 진정한 ‘전략적 협력관계’로 정의한 것이다. 한중 발표문은 그러나 양국이 사드문제를 우선 봉합해나간다는 데 합의했다는 점에서 의미가 있다. 양국 정부는 한중관계 개선의 필요성을 인식해 양측의 입장을 하는 수준에서 사드 문제를 매듭짓기로 했다. 서로의 ‘체면’은 살려주면서도 경색국면의 출구를 마련할 수 있는 토대를 마련한 것이다."

chars = list(set(data))
data_size, vocab_size = len(data), len(chars)
print('데이터는 {}개의 글자로 되어 있고, {}개의 고유한 문자가 있습니다.'.format(data_size, vocab_size))
char_to_ix = {ch: i for i, ch in enumerate(chars)}
ix_to_char = {i: ch for i, ch in enumerate(chars)}

hidden_size = 100
seq_length = 25
learning_rate = 1e-1

Wxh = np.random.randn(hidden_size, vocab_size) * 0.01
Whh = np.random.randn(hidden_size, hidden_size) * 0.01
Why = np.random.randn(vocab_size, hidden_size) * 0.01
bh = np.zeros((hidden_size, 1))
by = np.zeros((vocab_size, 1))
def lossFun(inputs, targets, hprev):
    xs, hs, ys, ps = {}, {}, {}, {}
    hs[-1] = np.copy(hprev)
    loss = 0

    # forward pass
    for t in range(len(inputs)):
        xs[t] = np.zeros((vocab_size, 1))
        xs[t][inputs[t]] = 1
        hs[t] = np.tanh(np.dot(Wxh, xs[t]) + np.dot(Whh, hs[t - 1]) + bh)
        ys[t] = np.dot(Why, hs[t]) + by
        ps[t] = np.exp(ys[t]) / np.sum(np.exp(ys[t]))
        loss += -np.log(ps[t][targets[t], 0])

    # backward pass
    dWxh, dWhh, dWhy = np.zeros_like(Wxh), np.zeros_like(Whh), np.zeros_like(Why)
    dbh, dby = np.zeros_like(bh), np.zeros_like(by)
    dhnext = np.zeros_like(hs[0])

    for t in reversed(range(len(inputs))):
        dy = np.copy(ps[t])
        dy[targets[t]] -= 1
        dWhy += np.dot(dy, hs[t].T)
        dby += dy
        dh = np.dot(Why.T, dy) + dhnext
        dhraw = (1 - hs[t] * hs[t]) * dh
        dbh += dhraw
        dWxh += np.dot(dhraw, xs[t].T)
        dWhh += np.dot(dhraw, hs[t - 1].T)
        dhnext = np.dot(Whh.T, dhraw)

    for dparam in [dWxh, dWhh, dWhy, dbh, dby]:
        np.clip(dparam, -5, 5, out=dparam)

    return loss, dWxh, dWhh, dWhy, dbh, dby, hs[len(inputs) - 1]

def sample(h, seed_ix, n):
    x = np.zeros((vocab_size, 1))
    x[seed_ix] = 1
    ixes = []
    for t in range(n):
        h = np.tanh(np.dot(Wxh, x) + np.dot(Whh, h) + bh)
        y = np.dot(Why, h) + by
        p = np.exp(y) / np.sum(np.exp(y))
        ix = np.random.choice(range(vocab_size), p=p.ravel())
        x = np.zeros((vocab_size, 1))
        x[ix] = 1
        ixes.append(ix)
    return ixes

n, p = 0, 0
mWxh, mWhh, mWhy = np.zeros_like(Wxh), np.zeros_like(Whh), np.zeros_like(Why)
mbh, mby = np.zeros_like(bh), np.zeros_like(by)
smooth_loss = -np.log(1.0 / vocab_size) * seq_length

while True:
    if p + seq_length + 1 >= len(data) or n == 0:
        hprev = np.zeros((hidden_size, 1))
        p = 0
    inputs = [char_to_ix[ch] for ch in data[p:p + seq_length]]
    targets = [char_to_ix[ch] for ch in data[p + 1:p + seq_length + 1]]

    if n % 100 == 0:
        sample_ix = sample(hprev, inputs[0], 200)
        txt = ''.join(ix_to_char[ix] for ix in sample_ix)
        print('----\n %s \n----' % (txt,))

    loss, dWxh, dWhh, dWhy, dbh, dby, hprev = lossFun(inputs, targets, hprev)
    smooth_loss = smooth_loss * 0.999 + loss * 0.001
    if n % 100 == 0: print('iter %d, loss: %f' % (n, smooth_loss))

    for param, dparam, mem in zip([Wxh, Whh, Why, bh, by],
                                  [dWxh, dWhh, dWhy, dbh, dby],
                                  [mWxh, mWhh, mWhy, mbh, mby]):
        mem += dparam * dparam
        param += -learning_rate * dparam / np.sqrt(mem + 1e-8)
    p += seq_length
    n += 1

    if n > 1500:
        break

start_string = "한국과 중국 정부는"

sample_ix = sample(hprev, char_to_ix[start_string[-1]], 200)
generated_text = start_string
for ix in sample_ix:
    generated_text += ix_to_char[ix]
print("Generated Text:")
print(generated_text)

input_sequence = "한국과 중국 정부는 "
input_ix = [char_to_ix[ch] for ch in input_sequence]
probs = sample(hprev, input_ix[-1], vocab_size)
next_word_ix = np.argmax(probs)
next_word = ix_to_char[next_word_ix]

real_next_word = data[len(input_sequence)]

print("Next Predicted Word after '{}' is '{}'".format(input_sequence, next_word))
print("Real Next Word is '{}'".format(real_next_word))

if next_word != real_next_word:
    print("예측이 올바르지 않습니다. n 값을 증가시켜보세요.")