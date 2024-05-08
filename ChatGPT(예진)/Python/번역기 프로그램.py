import http.client
import urllib.parse
import json


def translate_text(text, dest_language='en'):
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
        'tl': dest_language,  # 번역할 언어
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
    # 사용자로부터 번역할 텍스트와 번역 대상 언어 입력 받기
    text = input("번역할 텍스트를 입력하세요: ")
    print("번역하고자 하는 언어를 선택하세요.")
    print("1. 영어(en) 2. 한국어(ko) 3. 일본어(ja) 4. 중국어(간체)(zh-CN) 5. 스페인어(es) 6. 프랑스어(fr) 7. 독일어(de) 8. 이탈리아어(it)")
    dest_language = input("언어 코드를 입력하세요: ")

    # 번역 함수 호출하여 번역 결과 받기
    translated_text = translate_text(text, dest_language)

    # 번역 결과 출력
    if translated_text:
        print("번역 결과:", translated_text)


if __name__ == "__main__":
    main()