const axios = require("axios");

const fs = require("fs");
const { type } = require("os");

const DB = "dataset/DB_stores.txt";

let menuUrl = (menu) => {
  return `https://map.naver.com/p/api/search/allSearch?query=${menu}&type=all&searchCoord=126.842362%3B35.219526&boundary=`;
};

fs.writeFileSync(DB, "", (err) => console.log(err));
//fs.appendFileSync(DB, url2, (err) => console.log(err));
const crawlData = async (menu) => {
  try {
    const headers = {
      Accept: "application/json",
      "Accept-Encoding": "identity",
    };
    const headers1 = {};
    const response = await axios.get(menuUrl(menu), {
      headers: headers1,
    });
    //console.log(response.data);
    const storeList = response.data.result.place.list;
    //console.log(storeList[1]);
    //console.log(response.data.result.place.list[0]);

    for (storeInfo in storeList) {
      const hasMenu = response.data.result.place.list[storeInfo].menuExist;

      if (hasMenu === "1") {
        const storeIndex = response.data.result.place.list[storeInfo].index;
        const storeName = response.data.result.place.list[storeInfo].name;

        const storeTel = response.data.result.place.list[storeInfo].tel;
        const storeMenuInfo =
          response.data.result.place.list[storeInfo].menuInfo;
        let storeCategory = response.data.result.place.list[storeInfo].category;
        const reviewCount =
          response.data.result.place.list[storeInfo].reviewCount;

        fs.appendFileSync(DB, storeName + "\t", (err) => console.log(err));
        let category = "";
        storeCategory.forEach((element) => {
          //현재 가게별 카테고리 분류가 안되는 상황. 한식인데 CHINESE로 라벨링 됨. 수정 요망.
          ///*
          //console.log(element);
          if (
            (!element.indexOf("한식") || !element.indexOf("국밥")) &&
            category.indexOf("Korean")
          ) {
            category += "Korean,";
          } else if (!element.indexOf("중식") && category.indexOf("Chinese")) {
            category += "Chinese,";
          } else if (!element.indexOf("일식") && category.indexOf("Japanese")) {
            category += "Japanese,";
          } else if (
            (!element.indexOf("카페") ||
              !element.indexOf("디저트") ||
              !element.indexOf("브런치") ||
              !element.indexOf("빵")) &&
            category.indexOf("Dessert")
          ) {
            category += "Dessert,";
          } else if (
            (!element.indexOf("이탈리안") ||
              !element.indexOf("이탈리아") ||
              !element.indexOf("파스타") ||
              !element.indexOf("양식")) &&
            category.indexOf("Western")
          ) {
            category += "Western,";
          } else if (
            (!element.indexOf("패스트푸드") ||
              !element.indexOf("햄버거") ||
              !element.indexOf("피자") ||
              !element.indexOf("치킨")) &&
            category.indexOf("Fastfood")
          ) {
            category += "Fastfood,";
          } else if (
            (!element.indexOf("분식") || !element.indexOf("떡볶이")) &&
            category.indexOf("Boonsik")
          ) {
            category += "Boonsik,";
          } else if (!category.indexOf("etc")) {
          } else {
            category += "etc,";
          }

          //console.log("After: " + element);
          //*/
        });
        fs.appendFileSync(
          DB,
          category.substring(0, category.length - 1) + "\n",
          (err) => console.log(err)
        );
        //fs.appendFileSync(DB, storeIndex + "번\t", (err) => console.log(err));

        fs.appendFileSync(DB, storeTel + "\t", (err) => console.log(err));
        fs.appendFileSync(DB, reviewCount + "\n", (err) => console.log(err));
        fs.appendFileSync(DB, storeMenuInfo + "\n\n", (err) =>
          console.log(err)
        );
      }
    }

    return response;
  } catch (error) {
    //console.error("Can't fetch the storeInfo", error.message);
    return null;
  }
};
crawlData("제육볶음");

let menus = [
  "제육볶음",
  "햄버거",
  "돌솥",
  "돈까스",
  "파스타",
  "짜장면",
  "국밥",
  "찌개",
  "보쌈",
  "초밥",
  "스시",
  "해장국",
  "닭갈비",
  "갈비탕",
  "라멘",
  "냉면",
  "순두부",
  "낙지",
  "카레",
  "삼계탕",
  "볶음밥",
  "국수",
  "삼겹살",
  "스테이크",
  "대게",
  "곱창",
  "육회",
  "찜닭",
  "장어",
  "족발",
  "찜",
  "만두",
];

/*
crawlData("햄버거")
  .then(crawlData("돌솥"))
  .then(crawlData("돈까스"))
  .then(crawlData("파스타"))
  .then(crawlData("짜장면"))
  .then(crawlData("국밥"))
  .then(crawlData("찌개"))
  .then(crawlData("보쌈"))
  .then(crawlData("초밥"))
  .then(crawlData("스시"));
*/
///*
crawlData("햄버거");
crawlData("돌솥");
crawlData("돈까스");
crawlData("파스타");
crawlData("짜장면");
crawlData("국밥");
crawlData("찌개");
crawlData("보쌈");
crawlData("초밥");
crawlData("스시");
crawlData("해장국");
crawlData("닭갈비");
crawlData("갈비탕");
crawlData("라멘");
crawlData("냉면");
crawlData("순두부");
crawlData("낙지");
crawlData("카레");
crawlData("삼계탕");
crawlData("볶음밥");
crawlData("국수");
crawlData("삼겹살");
crawlData("스테이크");
crawlData("대게");
crawlData("곱창");
crawlData("육회");
crawlData("찜닭");
crawlData("장어");
crawlData("족발");
crawlData("찜");
crawlData("만두");
//*/
