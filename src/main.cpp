#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;
using namespace geode::log;

enum ResID {
	GOLD_COIN = 142,
	SILVER_COIN = 1329
};

class $modify(PlayLayer) {

	struct Fields {
		std::vector<std::tuple<GameObject*, CCSprite*, CCSprite*>> m_coinStore;
		std::vector<std::tuple<CheckpointObject*, CCSprite*>> m_checkpointStore;
		int checkpointCount = 0;
	};

	void rebuildCheckpoints() {

		// remove old sprites from progress bar
		for (auto [chk, sprite] : m_fields->m_checkpointStore) {
			if (sprite) sprite->removeFromParentAndCleanup(true);
		}

		m_fields->m_checkpointStore.clear();

		for (auto* chk : CCArrayExt<CheckpointObject>(m_checkpointArray)) {

			auto obj = chk->getObject();
			if (!obj) continue;

			auto checkpoint = CCSprite::createWithSpriteFrameName("checkpoint_01_001.png");
			checkpoint->setScale(0.35f);
			checkpoint->setAnchorPoint({ 0.5f, 0.5f });
			m_progressBar->addChild(checkpoint);

			m_fields->m_checkpointStore.push_back({ chk, checkpoint });
		}
	}

	void updateProg() {

		// Coins sections
		for (auto [obj, sprite, laneSprite] : m_fields->m_coinStore) {

			auto newX = obj->getPositionX() / m_levelLength * m_progressBar->getContentWidth();
			auto newY = -5.f;

			sprite->setPositionX(newX);
			sprite->setPositionY(newY);

			auto cache = CCSpriteFrameCache::sharedSpriteFrameCache();
			sprite->setDisplayFrame(
				cache->spriteFrameByName(fmt::format(
					"secretCoin{}{}_01_001.png",
					obj->m_objectID == SILVER_COIN ? "_2" : "",
					obj->m_isDisabled ? "_b" : ""
				).c_str())
			);

			laneSprite->setPositionX(newX);
			laneSprite->setPositionY(m_progressBar->getContentHeight() / 2);
		}

		//Checkpoint sections
		int count = m_checkpointArray->count();

		if (count != m_fields->checkpointCount) {
			m_fields->checkpointCount = count;
			rebuildCheckpoints();

			for (auto [chk, sprite] : m_fields->m_checkpointStore) {

				auto obj = chk->getObject();
				if (!obj) continue;

				auto newX = obj->getPositionX() / m_levelLength * m_progressBar->getContentWidth();
				auto newY = m_progressBar->getContentHeight() / 2;

				sprite->setPositionX(newX);
				sprite->setPositionY(newY);
			}
		}
	}

	void setupHasCompleted() {
		PlayLayer::setupHasCompleted();

		for (auto& obj : CCArrayExt<GameObject>(m_objects)) {

			if (obj->m_objectID == GOLD_COIN || obj->m_objectID == SILVER_COIN) {

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

		updateProg();
	}

	void postUpdate(float dt) {
		PlayLayer::postUpdate(dt);
		updateProg();
	}
};