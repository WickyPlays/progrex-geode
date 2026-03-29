#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace geode::log;

enum CoinID {
	GOLD = 142,
	SILVER = 1329
};

class $modify(PlayLayer) {

	//	Fields: (GameObject*, coinSprite, lineSprite)
	struct Fields {
		std::vector<std::tuple<GameObject*, CCSprite*, CCSprite*>> m_coinStore;
	};

	void updateCoins() {
		for (auto [obj, sprite, laneSprite] : m_fields->m_coinStore) {

			auto newX = obj->getPositionX() / m_levelLength * m_progressBar->getContentWidth();
			auto newY = -5.f;

			sprite->setPositionX(newX);
			sprite->setPositionY(newY);

			auto cache = CCSpriteFrameCache::sharedSpriteFrameCache();
			sprite->setDisplayFrame(
				cache->spriteFrameByName(fmt::format(
					"secretCoin{}{}_01_001.png", 
					obj->m_objectID == SILVER ? "_2" : "",
					obj->m_isDisabled ? "_b" : ""
				).c_str()));

			// line goes inside progress bar
			laneSprite->setPositionX(newX);
			laneSprite->setPositionY(m_progressBar->getContentHeight() / 2);
		}
	}

	void setupHasCompleted() {
		PlayLayer::setupHasCompleted();

		for (auto& obj : CCArrayExt<GameObject>(m_objects)) {
			//We are looping ALL m_objects instead. I don't even understand how m_coinArray even works here...
			if (obj->m_objectID == GOLD || obj->m_objectID == SILVER) {

				auto coin = CCSprite::createWithSpriteFrameName("secretCoin_01_001.png");
				coin->setScale(0.25);
				m_progressBar->addChild(coin);

				auto line = CCSprite::create("square.png");
				line->setAnchorPoint({ 0.5f, 0.5f });
				line->setScaleX(0.1f);
				line->setScaleY(2.f);
				line->setOpacity(200);
				m_progressBar->addChild(line);
				m_fields->m_coinStore.push_back({ obj, coin, line });
			}
		}

		updateCoins();
	}

	void postUpdate(float dt) {
		PlayLayer::postUpdate(dt);
		updateCoins();
	}
};