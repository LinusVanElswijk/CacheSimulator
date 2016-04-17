#pragma once

#include "Events.h"
#include "observing/Observer.h"
#include "CacheSimulator.h"
#include <string>
#include <vector>
#include <ostream>
#include <map>
#include <iomanip>

namespace cache_simulation {
	class CacheSimulatorLogger {
	private:
		class ComponentLogger
		: public observing::Observer<BlockReadEvent>
		, public observing::Observer<BlockWriteEvent> {
		public:
			virtual void onEvent(const BlockReadEvent& readEvent) {
				accessCount++;
				readCount++;

				if (readEvent.cacheHit) { 
					hitCount++;
				}
				else {
					missCount++;
				}
			}

			virtual void onEvent(const BlockWriteEvent& writeEvent) {
				accessCount++;
				writeCount++;

				if (writeEvent.cacheHit) {
					hitCount++;
				}
				else {
					missCount++;
				}
			}

			void outputTo(std::ostream& outStream, MemoryView& ram, int componentLatency, std::uint64_t totalLatency) {
				outStream << "=================== RAM ======================\n" <<
					"INFORMATION\n" <<
					"    block size  : " << std::setw(15) << ram.blockSize() << " bytes\n" <<
					"    latency     : " << std::setw(15) << componentLatency << " cycles\n" <<
					"ACCESS\n" <<
					"    access count: " << std::setw(15) << accessCount << "\n" <<
					"    read count  : " << std::setw(15) << readCount << " ( " << std::setw(9) << std::fixed << std::setprecision(8) << (double(readCount) / (double)accessCount) << " )\n" <<
					"    write count : " << std::setw(15) << writeCount << " ( " << std::setw(9) << std::fixed << std::setprecision(8) << (double(writeCount) / (double)accessCount) << " )\n" <<
					"    hit count   : " << std::setw(15) << hitCount << " ( " << std::setw(9) << std::fixed << std::setprecision(8) << (double(hitCount) / (double)accessCount) << " )\n" <<
					"    miss count  : " << std::setw(15) << missCount << " ( " << std::setw(9) << std::fixed << std::setprecision(8) << (double(missCount) / (double)accessCount) << " )\n" <<
					"TIME SPENT ON COMPONENT\n" <<
					"    sum latency : " << std::setw(15) << accessCount * componentLatency << " cycles ( " << std::setw(9) << std::fixed << std::setprecision(8) << (double(componentLatency) / (double)totalLatency) << " )\n"
					;
			}

			void outputTo(std::ostream& outStream, const int level, NWayAssociativeCache& cache, int componentLatency, std::uint64_t totalLatency) {
				outStream << "=================== L" << level << " ======================\n" <<
					"INFORMATION\n" <<
					"    cache size  : " << std::setw(15) << cache.setSize() * cache.setCount() * cache.blockSize() << " bytes\n" <<
					"    block size  : " << std::setw(15) << cache.blockSize() << " bytes\n" <<
					"    set count   : " << std::setw(15) << cache.setCount()  << "\n" <<
					"    set size    : " << std::setw(15) << cache.setSize() << "\n" <<
					"    latency     : " << std::setw(15) << componentLatency << " cycles\n" <<
					"ACCESS\n" <<
					"    access count: " << std::setw(15) << accessCount << "\n" <<
					"    read count  : " << std::setw(15) << readCount   << " ( " << std::setw(9) << std::fixed << std::setprecision(8) << (double(readCount) / (double)accessCount)  << " )\n" <<
					"    write count : " << std::setw(15) << writeCount  << " ( " << std::setw(9) << std::fixed << std::setprecision(8) << (double(writeCount) / (double)accessCount) << " )\n" <<
					"    hit count   : " << std::setw(15) << hitCount    << " ( " << std::setw(9) << std::fixed << std::setprecision(8) << (double(hitCount) / (double)accessCount)   << " )\n" <<
					"    miss count  : " << std::setw(15) << missCount   << " ( " << std::setw(9) << std::fixed << std::setprecision(8) << (double(missCount) / (double)accessCount)  << " )\n" <<
					"TIME SPENT ON COMPONENT\n" << 
					"    sum latency : " << std::setw(15) << accessCount * componentLatency << " cycles ( " << std::setw(9) << std::fixed << std::setprecision(8) << (double(componentLatency) / (double)totalLatency) << " )\n"
				;
			}

			std::int64_t getAccessCount() const { return accessCount; }
		private:

			std::int64_t accessCount = 0;
			std::int64_t readCount   = 0;
			std::int64_t writeCount  = 0;
			std::int64_t hitCount	 = 0;
			std::int64_t missCount	 = 0;
		};

	public:
		explicit CacheSimulatorLogger(CacheSimulator& simulator)
		:simulator(simulator)
		, componentLoggers_()
		{

			for (int i = 1; i <= simulator.levelCount(); i++) {
				componentLoggers_.emplace_back(new ComponentLogger());
				simulator.level(i).blockReading().attach(componentLoggers_.back());
				simulator.level(i).blockWriting().attach(componentLoggers_.back());
			}
			componentLoggers_.emplace_back(new ComponentLogger());
			simulator.ram().blockReading().attach(componentLoggers_.back());
			simulator.ram().blockWriting().attach(componentLoggers_.back());
		}

		void outputTo(std::ostream& outStream) {
			std::uint64_t totalLatency = 0;
			for (int i = 1; i <= simulator.levelCount(); i++) {
				totalLatency += (componentLoggers_[i - 1]->getAccessCount() * simulator.levelLatency(i));
			}

			outStream << "total nr of cycles spent : " << totalLatency << "\n";

			for (int i = 1; i <= simulator.levelCount(); i++) {
				componentLoggers_[i - 1]->outputTo(outStream, i, simulator.level(i), simulator.levelLatency(i), totalLatency);
			}

			componentLoggers_.back()->outputTo(outStream, simulator.ram(), simulator.ramLatency(), totalLatency);
		}

	private:
		std::vector<std::shared_ptr<ComponentLogger>> componentLoggers_;

		CacheSimulator& simulator;
	};
}