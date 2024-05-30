import http.client
import urllib.parse
import json


def translate_text(text):
    # Google Translate API 서버 정보
    api_host = 'translate.googleapis.com'
    api_path = '/translate_a/single'
    api_key = None  # Google Translate API는 API 키가 필요하지 않습니다.

    # HTTP 연결 설정
    connection = http.client.HTTPSConnection(api_host)

    # 번역 요청에 필요한 매개변수 설정
    params = {
        'client': 'gtx',  # Google Translate 클라이언트
        'sl': 'auto',  # 원본 언어는 자동 감지
        'tl': 'en',  # 영어로 번역
        'dt': 't',  # 결과 형식은 텍스트로 설정
        'q': text  # 번역할 텍스트
    }

    # API 경로에 맞게 요청 URL 구성
    url = f"{api_path}?{urllib.parse.urlencode(params)}"

    # API로 요청 보내기
    connection.request('GET', url)
    response = connection.getresponse()

    # 응답 코드 확인
    if response.status == 200:
        # 응답 데이터에서 번역 결과 추출
        data = response.read().decode('utf-8')
        translated_text = json.loads(data)[0][0][0]
        return translated_text
    else:
        print("번역에 실패했습니다.")
        return None


def main():
    # 사용자로부터 번역할 텍스트 입력 받기
    text = input("번역할 자연어 텍스트를 입력하세요: ")

    # 번역 함수 호출하여 번역 결과 받기
    translated_text = translate_text(text)

    # 번역 결과 출력
    if translated_text:
        print("자연어를 영어로 번역한 결과:", translated_text)


if __name__ == "__main__":
    main()
